#include "../base/log.hh"
#include "ogldevice.hh"
#include "oglcubemap.hh"
#include "tooglconvs.hh"

namespace ion {
namespace opengldrv {

	#define ISPOW2(n) ( !( (n) & ((n)-1) ))
	
	inline ion_uint32 nextpow2(const ion_uint32 n)
	{
		ion_uint32 exponent=((ion_uint32)( logf((float)n)/logf(2.0f) )) + 1;
		return 1<<exponent;
	}



	void OGLCubemap::OGLCubeSurface::bind() const
	{
		if (m_DepthRenderbuffer==0) {
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_Framebuffer);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,m_Face,m_GLHandle,m_Level);
		} else {
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,m_DepthRenderbuffer);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,m_DepthRenderbuffer);
		}
	}

	void OGLCubemap::OGLCubeSurface::unbind() const
	{
		if (m_DepthRenderbuffer==0) {
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
		} else {
			glBindFramebufferEXT(GL_RENDERBUFFER_EXT,0);
		}
	}



	OGLCubemap::OGLCubemap(OGLDevice& ogldevice,const base::String& identifier,
		const ion_uint32 edgelength,const ion_uint32 levels,
		const ion_uint32 flags,const video::Pixelformat format,const video::Memorypool pool):Cubemap(format,identifier),
		m_rOGLDevice(ogldevice),m_IsValid(true),m_IsDataOK(true),m_IsMapped(false),m_GLHandle(0),m_Framebuffer(0),
		m_DepthRenderbuffer(0),m_StencilRenderbuffer(0),m_LevelEdgelength(0),m_MappedLevel(0),
		m_CurrentLevel(0xffffffff),m_CurrentFace(FrontFace)
	{
		if (!video::isDepthformat(format)) {
			glGenTextures(1,&m_GLHandle);
		}

		m_pSurfaces=0;

		m_OGLTexture.target(GL_TEXTURE_CUBE_MAP_ARB);
		m_OGLTexture.handle(m_GLHandle);

		ion_uint32 actualedgelength=edgelength;

		if (!ISPOW2(actualedgelength)) actualedgelength=nextpow2(actualedgelength);

		m_pPicbuffer= ::new video::SimplePicbuffer(edgelength,edgelength,ogl2dtexformat(format));

		m_Edgelength=actualedgelength;
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
					glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT24_ARB,actualedgelength,actualedgelength);
					glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);

					m_NumMipmaplevels=1;

					m_pSurfaces=new OGLCubeSurface[6];
					m_pSurfaces[0].m_pOGLCubemap=this;
					m_pSurfaces[0].m_pTexture=this;
					m_pSurfaces[0].m_Level=0;
					m_pSurfaces[0].m_GLHandle=0;
					m_pSurfaces[0].m_Width=actualedgelength;
					m_pSurfaces[0].m_Height=actualedgelength;
					m_pSurfaces[0].m_DepthRenderbuffer=m_DepthRenderbuffer;
					m_pSurfaces[0].m_Framebuffer=0;
					m_pSurfaces[0].m_StencilRenderbuffer=0;
					m_pSurfaces[0].m_Face=tooglcubemapfaces((Cubemap::Face)0);
				} else m_IsValid=false;
			} else if (flags&video::Textureflag_IsRendertaget) {
				glGenFramebuffersEXT(1,&m_Framebuffer);

				const GLenum glcubemapface[]={
					GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
					GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB,
					GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB,
					GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB,
					GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB,
					GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB
				};

				glBindTexture(GL_TEXTURE_CUBE_MAP_ARB,m_GLHandle); // TODO: Save bound textures and restore them afterwards

				GLenum format=oglrgbaformat(video::rgbalayoutFromPixelformat(m_Pixelformat));
				GLenum type=GL_UNSIGNED_BYTE; // TODO

				m_NumMipmaplevels=1; // TODO: Support mipmaps in rendertargets

				m_pSurfaces=new OGLCubeSurface[6];

				for (int i=0;i<6;++i) {
					m_pSurfaces[i].m_pOGLCubemap=this;
					m_pSurfaces[i].m_pTexture=this;
					m_pSurfaces[i].m_Level=0;
					m_pSurfaces[i].m_GLHandle=m_GLHandle;
					m_pSurfaces[i].m_Width=actualedgelength;
					m_pSurfaces[i].m_Height=actualedgelength;
					m_pSurfaces[i].m_DepthRenderbuffer=0;
					m_pSurfaces[i].m_Framebuffer=m_Framebuffer;
					m_pSurfaces[i].m_StencilRenderbuffer=0;
					m_pSurfaces[i].m_Face=tooglcubemapfaces((Cubemap::Face)i);
					glTexImage2D(glcubemapface[i],0,m_Internalformat,actualedgelength,actualedgelength,0,format,type,0);
				}

				glBindTexture(GL_TEXTURE_CUBE_MAP_ARB,0);
			}
		} else if (video::isDepthformat(format) || (flags&video::Textureflag_IsRendertaget)) {
			m_IsValid=false;
		}

		if (m_GLHandle!=0) {
			// BUG: GL_NEAREST_MIPMAP_NEAREST does not work
			glbind();
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_MIN_FILTER,(levels==1) ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			if (levels>1) glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB,GL_TEXTURE_MAX_LEVEL,levels-1);
		}

		if (levels==0) {
			if (m_Edgelength<=1) m_NumMipmaplevels=1;
			else {
				m_NumMipmaplevels=((ion_uint32)( logf((float)m_Edgelength)/logf(2.0f) )) + 1;
			}
		}

		m_OGLTexture.numLevels(m_NumMipmaplevels);

		m_pDataSubmitted=new bool[m_NumMipmaplevels];
		for (ion_uint32 lvl=0;lvl<m_NumMipmaplevels;++lvl) m_pDataSubmitted[lvl]=false;

		currentLevel(0);
	}

	OGLCubemap::~OGLCubemap()
	{
		unmap();
		if (m_pSurfaces) delete [] m_pSurfaces;
		delete [] m_pDataSubmitted;
		if (m_pPicbuffer!=0) ::delete m_pPicbuffer;
		if (m_GLHandle!=0) glDeleteTextures(1,&m_GLHandle);
		if ((glDeleteFramebuffersEXT!=0) && (m_Framebuffer!=0)) glDeleteFramebuffersEXT(1,&m_Framebuffer);
		if ((glDeleteRenderbuffersEXT!=0) && (m_DepthRenderbuffer!=0)) glDeleteRenderbuffersEXT(1,&m_DepthRenderbuffer);
	}

	void OGLCubemap::glbind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, m_GLHandle);
	}

	GLuint OGLCubemap::handle() const
	{
		return m_GLHandle;
	}

	void OGLCubemap::currentFace(const Face face)
	{
		m_CurrentFace=face;
	}

	OGLCubemap::Face OGLCubemap::currentFace() const
	{
		return m_CurrentFace;
	}

	ion_uint32 OGLCubemap::numMipmaplevels() const
	{
		return m_NumMipmaplevels;
	}

	void OGLCubemap::map()
	{
		if (m_IsMapped) return;
		m_MappedLevel=currentLevel();
		m_IsMapped=true;
	}

	void OGLCubemap::unmap()
	{
		if (!m_IsMapped) return;

		GLenum format=oglrgbaformat(video::rgbalayoutFromPixelformat(m_Pixelformat));
		GLenum type=GL_UNSIGNED_BYTE; // TODO


		//if (!m_pDataSubmitted[currentLevel()]) {
			glTexImage2D(tooglcubemapfaces(m_CurrentFace),(GLint)currentLevel(),m_Internalformat,
				m_Edgelength,m_Edgelength,0,format,type,pixeldata());
			m_pDataSubmitted[currentLevel()]=true;
		/*} else {
			glTexSubImage2D(tooglcubemapfaces(m_CurrentFace),(GLint)currentLevel(),0,0,m_Edgelength,m_Edgelength,
				format,type,pixeldata());
		}*/

		m_IsMapped=false;
	}

	void OGLCubemap::currentLevel(const ion_uint32 level)
	{
		if ((m_CurrentLevel==level) || m_IsMapped) return;

		m_CurrentLevel=level;

		m_LevelEdgelength=m_Edgelength/(1<<level); if (m_LevelEdgelength<1) m_LevelEdgelength=1;
	}

	ion_uint32 OGLCubemap::currentLevel() const
	{
		return m_CurrentLevel;
	}

	OGLCubemap::Texsurface* OGLCubemap::surface(const Face face,const ion_uint32 level)
	{
		return m_pSurfaces ? &(m_pSurfaces[level*6+((int)face)]) : 0;
	}

	bool OGLCubemap::isDataOK() const
	{
		return m_IsDataOK;
	}

	void OGLCubemap::dataIsOK()
	{
		m_IsDataOK=true;
	}

	void* OGLCubemap::texHandle() const
	{
		return (void*)(&m_OGLTexture);
	}

	bool OGLCubemap::pixelLittleEndian() const
	{
		return false;
	}

	ion_uint8* OGLCubemap::pixeldata()
	{
		return m_pPicbuffer->pixeldata();
	}

	const ion_uint8* OGLCubemap::pixeldata() const
	{
		return m_pPicbuffer->pixeldata();
	}

	ion_uint32 OGLCubemap::stride() const
	{
		return m_pPicbuffer->bytesPerPixel()*m_LevelEdgelength;
	}

	ion_uint32 OGLCubemap::bytesPerPixel() const
	{
		return m_pPicbuffer->bytesPerPixel();
	}

	ion_uint32 OGLCubemap::width() const
	{
		return m_LevelEdgelength;
	}

	ion_uint32 OGLCubemap::height() const
	{
		return m_LevelEdgelength;
	}

	bool OGLCubemap::isValid() const
	{
		return m_IsValid;
	}

	GLenum OGLCubemap::tooglcubemapfaces(const Face face)
	{
		switch (face) {
			case Cubemap::FrontFace:return GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB;
			case Cubemap::BackFace:return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB;

			case Cubemap::LeftFace:return GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB;
			case Cubemap::RightFace:return GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB;

			case Cubemap::TopFace:return GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB;
			case Cubemap::BottomFace:return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB;

			default:return GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB;
		}
	}

}
}
