#ifndef ION_VIDEO_FONT_HH_INCLUDED
#define ION_VIDEO_FONT_HH_INCLUDED

#include "../base/streamable.hh"
#include "../math/rectangle.hh"
#include "../math/transform.hh"
#include "videodevice.hh"
#include "draw2dcache.hh"

#ifdef ION_VIDEO_FONT_SYSTEM_IMPLEMENTATION
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

namespace ion {
namespace video {

	class ION_API Font
	{
	public:
		static int initFontsystem();
		static int shutdownFontsystem();

		static const char* defaultCharset();

		Font(Draw2DCache& cache);
		Font(Draw2DCache& cache,base::Streamable& streamable,const unsigned int height,const char *charset=0,
			const unsigned int horizdpi=72,const unsigned int vertdpi=72);
		~Font();
		void load(base::Streamable& streamable,const unsigned int height,const char *charset=0,
			const unsigned int horizdpi=72,const unsigned int vertdpi=72);

		void drawText(const base::String& text,const float x,const float y,const Draw2DCache::AddImageMode mode=Draw2DCache::AddImageMode_DirectCoordinates);
		void drawText(const base::String& text,const float x,const float y,const Color& color,const Draw2DCache::AddImageMode mode=Draw2DCache::AddImageMode_DirectCoordinates);
		void drawText(const base::String& text,const math::FloatRectangle& rectangle,const Color& color,
			const Draw2DCache::AddImageMode mode=Draw2DCache::AddImageMode_DirectCoordinates);

		//! Height, in pixels.
		unsigned int height() const;

		//! Width of the string, in pixels.
		unsigned int stringwidth(const base::String& text) const;

	protected:
		void cleanup();
		void generateGlyphs(const unsigned int height,const char *charset,
			const unsigned int horizdpi,const unsigned int vertdpi);

#ifdef ION_VIDEO_FONT_SYSTEM_IMPLEMENTATION
		static FT_Library g_FTLibrary;
#endif

		struct Glyphentry {
			bool m_Enabled;
			float m_U,m_V,m_UWidth,m_UHeight;
			int m_PixelWidth,m_HorizAdvance;
			int m_TopBearing,m_BottomBearing;
			long m_KerningOffsets[256];
		};

		Glyphentry m_Glyphs[256];
		Texture2D *m_pGlyphTexture;

		unsigned int m_PixelHeight;

		struct Fontdata;
		Fontdata *m_pFontdata;
		Draw2DCache* m_pDraw2DCache;
	};

}
}

#endif
