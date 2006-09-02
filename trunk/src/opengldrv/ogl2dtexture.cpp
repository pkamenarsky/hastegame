#include "../base/log.hh"
#include "ogldevice.hh"
#include "ogl2dtexture.hh"
#include "tooglconvs.hh"

namespace ion {
namespace opengldrv {

	#define ISPOW2(n) ( !( (n) & ((n)-1) ))
	
	inline ion_uint32 nextpow2(const ion_uint32 n)
	{
		ion_uint32 exponent=((ion_uint32)( logf((float)n)/logf(2.0f) )) + 1;
		return 1<<exponent;
	}


	void OGL2DTexture::OGL2DSurface::bind() const
	{
		if (m_DepthRenderbuffer==0) {
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_Framebuffer);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,m_GLHandle,m_Level);
		} else {
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,m_DepthRenderbuffer);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,m_DepthRenderbuffer);
		}
	}

	void OGL2DTexture::OGL2DSurface::unbind() const
	{
		if (m_DepthRenderbuffer==0) {
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
		} else {
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);
		}
	}



	OGL2DTexture::OGL2DTexture(OGLDevice& ogldevice,const base::String& identifier,
			const ion_uint32 width,const ion_uint32 height,const ion_uint32 levels,const ion_uint32 flags,const video::Pixelformat format,
			const video::Memorypool pool):video::Texture2D(format,identifier,flags),m_rOGLDevice(ogldevice),m_IsValid(true),
			m_IsDataOK(true),m_IsMapped(false),m_GLHandle(0),m_Framebuffer(0),m_DepthRenderbuffer(0),
			m_StencilRenderbuffer(0),m_LevelWidth(0),m_LevelHeight(0),m_MappedLevel(0),m_CurrentLevel(0xffffffff)/*,
			m_Picbuffer(width,height,ogl2dtexformat(format))*/
	{
		if (!video::isDepthformat(format)) {
			glGenTextures(1,&m_GLHandle);
		}

		m_pSurfaces=0;

		m_OGLTexture.target(GL_TEXTURE_2D);
		m_OGLTexture.handle(m_GLHandle);

		ion_uint32 actualwidth=width,actualheight=height;

		if (!ISPOW2(actualwidth)) actualwidth=nextpow2(actualwidth);
		if (!ISPOW2(actualheight)) actualheight=nextpow2(actualheight);

		if (actualwidth!=actualheight) {
			ion_uint32 i=(actualwidth>actualheight) ? actualwidth : actualheight;
			actualwidth=actualheight=i;
		}

		m_pPicbuffer= ::new video::SimplePicbuffer(actualwidth,actualheight,ogl2dtexformat(format));

		m_Width=actualwidth;
		m_Height=actualheight;
		m_Pixelformat=m_pPicbuffer->pixelformat();
		m_NumMipmaplevels=levels;

		m_Internalformat=oglpixelformat(format);

		if (ogldevice.extensionSupported("GL_EXT_framebuffer_object")) {
			if (video::isDepthformat(format)) {
				if (ogldevice.extensionSupported("GL_ARB_depth_texture")) {
					GLenum depthformat=GL_DEPTH_COMPONENT24_ARB;
					switch (format) {
						case video::Pixelformat_D15S1:
						case video::Pixelformat_D16:
							depthformat=GL_DEPTH_COMPONENT16_ARB;
							break;
						case video::Pixelformat_D24:
						case video::Pixelformat_D24S4:
						case video::Pixelformat_D24S8:
						case video::Pixelformat_FD24S8:
							depthformat=GL_DEPTH_COMPONENT24_ARB;
							break;
						case video::Pixelformat_D32:
							depthformat=GL_DEPTH_COMPONENT32_ARB;
							break;
						default:break;
					}
	
					// TODO: depthstencil formats
	
					glGenRenderbuffersEXT(1,&m_DepthRenderbuffer);
					glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,m_DepthRenderbuffer);
					glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT24_ARB,width,height);
					glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);

					m_NumMipmaplevels=1;

					m_pSurfaces=new OGL2DSurface[1];
					m_pSurfaces[0].m_pOGL2DTexture=this;
					m_pSurfaces[0].m_pTexture=this;
					m_pSurfaces[0].m_Level=0;
					m_pSurfaces[0].m_GLHandle=0;
					m_pSurfaces[0].m_Width=actualwidth;
					m_pSurfaces[0].m_Height=actualheight;
					m_pSurfaces[0].m_DepthRenderbuffer=m_DepthRenderbuffer;
					m_pSurfaces[0].m_Framebuffer=0;
					m_pSurfaces[0].m_StencilRenderbuffer=0;

				} else m_IsValid=false;
			} else if (flags&video::Textureflag_IsRendertaget) {
				glGenFramebuffersEXT(1,&m_Framebuffer);

				glBindTexture(GL_TEXTURE_2D,m_GLHandle); // TODO: Save bound textures and restore them afterwards

				GLenum format=oglrgbaformat(video::rgbalayoutFromPixelformat(m_Pixelformat));
				GLenum type=GL_UNSIGNED_BYTE; // TODO

				m_NumMipmaplevels=1; // TODO: Support mipmaps in rendertargets

				m_pSurfaces=new OGL2DSurface[1];
				m_pSurfaces[0].m_pOGL2DTexture=this;
				m_pSurfaces[0].m_pTexture=this;
				m_pSurfaces[0].m_Level=0;
				m_pSurfaces[0].m_GLHandle=m_GLHandle;
				m_pSurfaces[0].m_Width=actualwidth;
				m_pSurfaces[0].m_Height=actualheight;
				m_pSurfaces[0].m_DepthRenderbuffer=0;
				m_pSurfaces[0].m_Framebuffer=m_Framebuffer;
				m_pSurfaces[0].m_StencilRenderbuffer=0;

				glTexImage2D(GL_TEXTURE_2D,0,m_Internalformat,width,height,0,format,type,0);

				glBindTexture(GL_TEXTURE_2D,0);
			}
		} else if (video::isDepthformat(format) || (flags&video::Textureflag_IsRendertaget)) {
			m_IsValid=false;
		}

		if (m_GLHandle!=0) {
			glbind();
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,(levels==1) ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			if (levels>1) glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,levels-1);
		}

		if (m_NumMipmaplevels==0) {
			ion_uint32 length=(actualwidth<actualheight) ? actualwidth : actualheight;

			if (length<=1) m_NumMipmaplevels=1;
			else {
				m_NumMipmaplevels=((ion_uint32)( logf((float)length)/logf(2.0f) )) + 1;
			}
		}

		m_OGLTexture.numLevels(m_NumMipmaplevels);

		m_pDataSubmitted=new bool[m_NumMipmaplevels];
		for (ion_uint32 lvl=0;lvl<m_NumMipmaplevels;++lvl) m_pDataSubmitted[lvl]=false;

		currentLevel(0);
	}

	OGL2DTexture::~OGL2DTexture()
	{
		unmap();
		if (m_pSurfaces) delete [] m_pSurfaces;
		delete [] m_pDataSubmitted;
		if (m_pPicbuffer!=0) ::delete m_pPicbuffer;
		if (m_GLHandle!=0) glDeleteTextures(1,&m_GLHandle);
		if ((glDeleteFramebuffersEXT!=0) && (m_Framebuffer!=0)) glDeleteFramebuffersEXT(1,&m_Framebuffer);
		if ((glDeleteRenderbuffersEXT!=0) && (m_DepthRenderbuffer!=0)) glDeleteRenderbuffersEXT(1,&m_DepthRenderbuffer);
	}

	bool OGL2DTexture::isValid() const
	{
		return m_IsValid;
	}

	bool OGL2DTexture::isDataOK() const
	{
		return m_IsDataOK;
	}

	void OGL2DTexture::dataIsOK()
	{
		m_IsDataOK=true;
	}

	void* OGL2DTexture::texHandle() const
	{
		return (void*)(&m_OGLTexture);
	}

	ion_uint8* OGL2DTexture::pixeldata()
	{
		return m_pPicbuffer->pixeldata();
	}

	const ion_uint8* OGL2DTexture::pixeldata() const
	{
		return m_pPicbuffer->pixeldata();
	}

	ion_uint32 OGL2DTexture::stride() const
	{
		return m_pPicbuffer->bytesPerPixel()*m_LevelWidth;
	}

	ion_uint32 OGL2DTexture::bytesPerPixel() const
	{
		return m_pPicbuffer->bytesPerPixel();
	}

	bool OGL2DTexture::pixelLittleEndian() const
	{
		return false;
	}

	ion_uint32 OGL2DTexture::width() const
	{
		return m_LevelWidth;
	}

	ion_uint32 OGL2DTexture::height() const
	{
		return m_LevelHeight;
	}

	ion_uint32 OGL2DTexture::numMipmaplevels() const
	{
		return m_NumMipmaplevels;
	}

	void OGL2DTexture::map()
	{
		if (m_IsMapped) return;
		m_MappedLevel=currentLevel();
		m_IsMapped=true;
	}

	void OGL2DTexture::unmap()
	{
		if (!m_IsMapped) return;

		GLenum format=oglrgbaformat(video::rgbalayoutFromPixelformat(m_Pixelformat));
		GLenum type=GL_UNSIGNED_BYTE; // TODO


		if (!m_pDataSubmitted[currentLevel()]) {
			glTexImage2D(GL_TEXTURE_2D,(GLint)currentLevel(),m_Internalformat,
				m_LevelWidth,m_LevelHeight,0,format,type,pixeldata());
			m_pDataSubmitted[currentLevel()]=true;
		} else {
			glTexSubImage2D(GL_TEXTURE_2D,(GLint)currentLevel(),0,0,m_LevelWidth,m_LevelHeight,
				format,type,pixeldata());
		}

		m_IsMapped=false;
	}

	video::Texture::Texsurface* OGL2DTexture::surface(const ion_uint32 level)
	{
		return m_pSurfaces ? &(m_pSurfaces[level]) : 0;
	}

	void OGL2DTexture::currentLevel(const ion_uint32 level)
	{
		if ((m_CurrentLevel==level) || m_IsMapped) return;

		m_CurrentLevel=level;

		m_LevelWidth=m_Width/(1<<level); if (m_LevelWidth<1) m_LevelWidth=1;
		m_LevelHeight=m_Height/(1<<level); if (m_LevelHeight<1) m_LevelHeight=1;
	}

	ion_uint32 OGL2DTexture::currentLevel() const
	{
		return m_CurrentLevel;
	}

	void OGL2DTexture::copyFromFramebuffer(const ion_uint32 x,const ion_uint32 y)
	{
		glbind();

		if (!m_pDataSubmitted[currentLevel()]) {
			glCopyTexImage2D(GL_TEXTURE_2D,(GLint)currentLevel(),m_Internalformat,x,y,
				m_LevelWidth,m_LevelHeight,0);
			m_pDataSubmitted[currentLevel()]=true;
		} else {
			glCopyTexSubImage2D(GL_TEXTURE_2D,(GLint)currentLevel(),0,0,x,y,m_LevelWidth,m_LevelHeight);
		}
	}

	void OGL2DTexture::glbind()
	{
		glBindTexture(GL_TEXTURE_2D, m_GLHandle);
	}
	
	GLuint OGL2DTexture::handle() const
	{
		return m_GLHandle;
	}

	GLuint OGL2DTexture::framebuffer() const
	{
		return m_Framebuffer;
	}

	GLuint OGL2DTexture::depthRenderbuffer() const
	{
		return m_DepthRenderbuffer;
	}

	GLuint OGL2DTexture::stencilRenderbuffer() const
	{
		return m_StencilRenderbuffer;
	}

}
}
