#include <assert.h>
#include "../base/localfile.hh"
#include "../base/log.hh"
#include "../base/datablock.hh"
#include "../video/simplepicbuffer.hh"
#include "../video/picsaver.hh"

#include <stdio.h>

#define ION_VIDEO_FONT_SYSTEM_IMPLEMENTATION
#include "font.hh"

namespace ion {
namespace video {

	FT_Library Font::g_FTLibrary;

	int Font::initFontsystem()
	{
		return FT_Init_FreeType(&g_FTLibrary);
	}

	int Font::shutdownFontsystem()
	{
		return FT_Done_FreeType(g_FTLibrary);
	}

	const char* Font::defaultCharset()
	{
		return " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!\"�%&/()=?^�|<>,.;:-_#\'+*~\\@";
	}




	struct Font::Fontdata
	{
		FT_Face m_Face;
		bool m_Initialized;

		Fontdata():m_Initialized(false) {}
	};



	Font::Font(Draw2DCache& cache):m_pGlyphTexture(0),m_pFontdata(new Fontdata),m_pDraw2DCache(&cache)
	{
	}

	Font::Font(Draw2DCache& cache,base::Streamable& streamable,const unsigned int height,const char *charset,
		const unsigned int horizdpi,const unsigned int vertdpi):
	m_pGlyphTexture(0),m_pFontdata(new Fontdata),m_pDraw2DCache(&cache)
	{
		load(streamable,height,charset,horizdpi,vertdpi);
	}

	Font::~Font()
	{
		cleanup();
		delete m_pFontdata;
	}

	void Font::load(base::Streamable& streamable,const unsigned int height,const char *charset,
		const unsigned int horizdpi,const unsigned int vertdpi)
	{
		if (m_pFontdata->m_Initialized)
			cleanup();

		base::Datablock dblock;
		dblock.transferFrom(streamable);

		int ret=FT_New_Memory_Face(g_FTLibrary,dblock.data(),dblock.size(),0,&(m_pFontdata->m_Face));

		if (ret!=0) {
			if (ret==FT_Err_Unknown_File_Format)
				base::log("Font::load()",base::Error) << "Unknown font file format (source " << streamable.name() << " )\n";
			else
				base::log("Font::load()",base::Error) << "Freetype2 error code " << ret << " (source " << streamable.name() << " )\n";

			return;
		}

		generateGlyphs(height,(charset!=0) ? charset : defaultCharset(),horizdpi,vertdpi);

		m_pFontdata->m_Initialized=true;
	}

	void Font::cleanup()
	{
		if (m_pGlyphTexture) delete m_pGlyphTexture; m_pGlyphTexture=0;

		if (m_pFontdata->m_Initialized) {
			FT_Done_Face(m_pFontdata->m_Face);
		}
		m_pFontdata->m_Initialized=false;
	}

	void Font::generateGlyphs(const unsigned int height,const char *charset,
		const unsigned int horizdpi,const unsigned int vertdpi)
	{
		int i;
		SimplePicbuffer *glyphbitmaps[256];

		for (i=0;i<256;++i) {
			for (int j=0;j<256;++j) m_Glyphs[i].m_KerningOffsets[j]=0;
			m_Glyphs[i].m_Enabled=false; glyphbitmaps[i]=0;
		}


		FT_Set_Char_Size(m_pFontdata->m_Face,
			0,
			height*64,
			horizdpi,
			vertdpi
			);

		const unsigned char *c=(const unsigned char *)charset;
		unsigned int totalwidth=0;
		int maxtopbearing=0,maxbottombearing=0;

		m_PixelHeight=0;

		bool hasKerning=FT_HAS_KERNING(m_pFontdata->m_Face)!=0;

		for (;*c;++c) {

			int idx=(*c);
			if (idx>255) continue; // Unicode not supported yet
			if (glyphbitmaps[idx]!=0) continue; // Glyph already processed

			FT_Load_Glyph(
				m_pFontdata->m_Face,
				FT_Get_Char_Index(m_pFontdata->m_Face,idx),
				FT_LOAD_DEFAULT
				);

			FT_Render_Glyph(
				m_pFontdata->m_Face->glyph,
				FT_RENDER_MODE_NORMAL
				);

			// Store the kerning values if available
			if (hasKerning) {
				const unsigned char *cc=(const unsigned char *)charset;
				for (;*cc;++cc) {
					int idx2=(*cc);
					if (idx2>255) continue;

					// for FT kerning, *cc is left and *c right, i.e.
					// kerning goes: glyphs[current].kerningoffsets[previous]

					FT_Vector delta;
					FT_Get_Kerning(m_pFontdata->m_Face,idx2,idx,FT_KERNING_DEFAULT,&delta);

					m_Glyphs[idx].m_KerningOffsets[*cc]=(delta.x)>>6;
				}
			}

			SimplePicbuffer *pBuf=new SimplePicbuffer(
				m_pFontdata->m_Face->glyph->bitmap.width,
				m_pFontdata->m_Face->glyph->bitmap.rows,
				video::Pixelformat_L8);

			glyphbitmaps[idx]=pBuf;
			m_Glyphs[idx].m_Enabled=true;
			m_Glyphs[idx].m_HorizAdvance=m_pFontdata->m_Face->glyph->metrics.horiAdvance>>6;
			m_Glyphs[idx].m_TopBearing=m_pFontdata->m_Face->glyph->metrics.horiBearingY/64;
			m_Glyphs[idx].m_BottomBearing=m_pFontdata->m_Face->glyph->bitmap.rows - m_Glyphs[idx].m_TopBearing;
			maxtopbearing=(maxtopbearing>m_Glyphs[idx].m_TopBearing) ? maxtopbearing : m_Glyphs[idx].m_TopBearing;
			maxbottombearing=(maxbottombearing>m_Glyphs[idx].m_BottomBearing) ? maxbottombearing : m_Glyphs[idx].m_BottomBearing;

			//Copy the glyph bitmap into the picbuf
			{
				for(int y = 0; y < m_pFontdata->m_Face->glyph->bitmap.rows; y++) {
					for(int x = 0; x < m_pFontdata->m_Face->glyph->bitmap.width; x++) {
						int bitmapcoord=x+y*m_pFontdata->m_Face->glyph->bitmap.pitch;
						ion_uint8 value=((ion_uint8*)(m_pFontdata->m_Face->glyph->bitmap.buffer))[bitmapcoord];
						pBuf->putpixelUint8(x,y,value,0,0,0);
					}
				}
			}
		}

		m_PixelHeight=maxtopbearing+maxbottombearing;

		ion_uint32 length=32; // start at 32x32

		while (length<2048) {
			c=(const unsigned char *)charset;
			ion_uint32 xpos=1,ypos=1;
			bool fits=true;

			for (;*c;++c) {
				int idx=(*c);
				if (idx>255) continue; // Unicode not supported yet
				SimplePicbuffer *pBuf=glyphbitmaps[idx];
				assert(pBuf!=0);

				if ((xpos+pBuf->width()+1)>length) {
					xpos=1; ypos+=m_PixelHeight+1;
				} else xpos+=pBuf->width()+1;

				int bearing=(maxtopbearing-m_Glyphs[idx].m_TopBearing);
				if (bearing<0) bearing=0;

				if ((ypos+bearing+pBuf->height()+1)>length) { fits=false; break; }
			}

			if (fits) break;

			length<<=1;
		}

		SimplePicbuffer glyphatlas(length,length,video::Pixelformat_RGBA8888);
		glyphatlas.clear(255,255,255,0); // zero alpha, but white color, to prevent color bleeding
		//memset(glyphatlas.data(),0,glyphatlas.size());

		c=(const unsigned char *)charset;
		ion_uint32 xpos=1,ypos=1;
		for (;*c;++c) {
			int idx=(*c);
			if (idx>255) continue; // Unicode not supported yet
			SimplePicbuffer *pBuf=glyphbitmaps[idx];
			assert(pBuf!=0);

			if ((xpos+pBuf->width()+1)>length) {
				xpos=1; ypos+=m_PixelHeight+1;
			}

			int bearing=(maxtopbearing-m_Glyphs[idx].m_TopBearing);
			if (bearing<0) bearing=0;

			// Draw the glyph into the atlas
			{
				for (ion_uint32 y=0;y<pBuf->height();++y) {
					for (ion_uint32 x=0;x<pBuf->width();++x) {
						ion_uint8 r=0,g=0,b=0,a=0;
						pBuf->getpixelUint8(x,y, r,g,b,a);
						ion_int32 destx=x+xpos,desty=y+ypos+bearing;

						assert(destx>=0);
						assert(desty>=0);
						assert(destx<(ion_int32)(glyphatlas.width()));
						assert(desty<(ion_int32)(glyphatlas.height()));

						glyphatlas.putpixelUint8(destx,desty, 255,255,255,r); // r = alpha
					}
				}
			}

			// Calculate uv coords
			{
				float x1=(float)(xpos);
				float y1=(float)(ypos);
				float lengthf=(float)(length);

				m_Glyphs[idx].m_U=x1/(lengthf-0.0f);
				m_Glyphs[idx].m_V=y1/(lengthf-0.0f);

				m_Glyphs[idx].m_UWidth=(pBuf->width()-0)/(lengthf-0.0f);
				m_Glyphs[idx].m_UHeight=((float)m_PixelHeight-0)/(lengthf-0.0f);

				m_Glyphs[idx].m_PixelWidth=pBuf->width();
			}

			xpos+=pBuf->width()+1;

		}

		/*{
			base::Localfile outlf;
			outlf.open("fontoutput.png","wb");
			video::save2DPic(outlf,glyphatlas);
		}*/

		m_pGlyphTexture=m_pDraw2DCache->videodevice().create2DTexture(
			"fontglyphs",length,length,1,0,video::Pixelformat_RGBA8888,video::Mempool_Managed);
		m_pGlyphTexture->map();
		m_pGlyphTexture->transfer(glyphatlas);
		m_pGlyphTexture->unmap();

		for (i=0;i<256;++i) { if (glyphbitmaps[i]) delete glyphbitmaps[i]; }

	}

	void Font::drawText(const base::String& text,const float x,const float y,const Draw2DCache::AddImageMode mode)
	{
		drawText(text,x,y,Color::white(),mode);
	}

	void Font::drawText(const base::String& text,const float x,const float y,const Color& color,const Draw2DCache::AddImageMode mode)
	{
		if (m_pGlyphTexture==0) return;

		const std::string& str=text.STLstring();

		float xpos=x,ypos=y;

		if (mode==Draw2DCache::AddImageMode_ScreenNormalizedCoordinates) {
			float w=(float)(m_pDraw2DCache->videodevice().displaywidth()-1);
			float h=(float)(m_pDraw2DCache->videodevice().displayheight()-1);

			xpos=floorf(xpos*w);
			ypos=floorf(ypos*h);
		}

		int oldc=0;
		bool hasPrevious=false;

		for (std::string::const_iterator itc=str.begin();itc!=str.end();++itc) {
			int c=(*itc);
			if ((c<0) || (c>255)) continue;
			const Glyphentry& glyph=m_Glyphs[c];
			if (!glyph.m_Enabled) continue;

			math::FloatRectangle r;
			r.left(glyph.m_U);
			r.top(glyph.m_V);
			r.width(glyph.m_UWidth);
			r.height(glyph.m_UHeight);

			if (hasPrevious) xpos+=glyph.m_KerningOffsets[oldc];

			m_pDraw2DCache->addImage(*m_pGlyphTexture,color,&r,
				math::FloatRectangle(xpos,ypos,(float)(glyph.m_PixelWidth),(float)(m_PixelHeight)),
				Draw2DCache::AddImageMode_DirectCoordinates);

			xpos+=(float)(glyph.m_HorizAdvance);
			oldc=c;
			hasPrevious=true;
		}
	}

	void Font::drawText(const base::String& text,const math::FloatRectangle& rectangle,const Color& color,const Draw2DCache::AddImageMode mode)
	{
		if (m_pGlyphTexture==0) return;

		const std::string& str=text.STLstring();

		float xpos=rectangle.left(),ypos=rectangle.top(),width=rectangle.width(),height=rectangle.height();

		if ((width==0) && (height==0)) return;

		if (mode==Draw2DCache::AddImageMode_ScreenNormalizedCoordinates) {
			float w=(float)(m_pDraw2DCache->videodevice().displaywidth()-1);
			float h=(float)(m_pDraw2DCache->videodevice().displayheight()-1);

			xpos=floorf(xpos*w);
			ypos=floorf(ypos*h);
			width=floorf(width*w);
			height=floorf(height*h);
		}

		int oldc=0;
		bool hasPrevious=false;

		std::string::const_iterator itc;

		float actualwidth=0;

		for (itc=str.begin();itc!=str.end();++itc) {
			int c=(*itc);
			if ((c<0) || (c>255)) continue;
			const Glyphentry& glyph=m_Glyphs[c];
			if (!glyph.m_Enabled) continue;

			actualwidth+=(float)(glyph.m_HorizAdvance);
		}

		if (actualwidth==0) return;

		float xscale=0,yscale=0;
		if (width==0)
			xscale=yscale=height/((float)m_PixelHeight);
		else if (height==0)
			xscale=yscale=width/actualwidth;
		else {
			xscale=width/actualwidth;
			yscale=height/((float)m_PixelHeight);
		}

		for (itc=str.begin();itc!=str.end();++itc) {
			int c=(*itc);
			if ((c<0) || (c>255)) continue;
			const Glyphentry& glyph=m_Glyphs[c];
			if (!glyph.m_Enabled) continue;

			math::FloatRectangle r;
			r.left(glyph.m_U);
			r.top(glyph.m_V);
			r.width(glyph.m_UWidth);
			r.height(glyph.m_UHeight);

			if (hasPrevious) xpos+=glyph.m_KerningOffsets[oldc];

			m_pDraw2DCache->addImage(*m_pGlyphTexture,color,&r,
				math::FloatRectangle(xpos,ypos,(float)(glyph.m_PixelWidth)*xscale,m_PixelHeight*yscale),
				Draw2DCache::AddImageMode_DirectCoordinates);

			xpos+=(float)(glyph.m_HorizAdvance)*xscale;
			oldc=c;
			hasPrevious=true;
		}
	}

	unsigned int Font::height() const
	{
		return m_PixelHeight;
	}

	unsigned int Font::stringwidth(const base::String& text) const
	{
		if (m_pGlyphTexture==0) return 0;

		const std::string& str=text.STLstring();

		unsigned int width=0;

		int oldc=0;
		bool hasPrevious=false;

		for (std::string::const_iterator itc=str.begin();itc!=str.end();++itc) {
			int c=(*itc);
			if ((c<0) || (c>255)) continue;
			const Glyphentry& glyph=m_Glyphs[c];
			if (!glyph.m_Enabled) continue;

			if (hasPrevious) width+=glyph.m_KerningOffsets[oldc];

			width+=glyph.m_HorizAdvance;
			oldc=c;
			hasPrevious=true;
		}

		return width;
	}

}
}
