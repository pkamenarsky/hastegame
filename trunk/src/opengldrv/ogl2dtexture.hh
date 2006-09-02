#ifndef ION_OPENGLDRV_OGL2DTEXTURE_HH_INCLUDED
#define ION_OPENGLDRV_OGL2DTEXTURE_HH_INCLUDED

#include <set>
#include <vector>
#include "iongl.hh"
#include "../base/dll.hh"
#include "../math/matrix.hh"
#include "../video/mempool.hh"
#include "../video/pixelformat.hh"
#include "../video/simplepicbuffer.hh"
#include "../video/texture2d.hh"

#include "oglsurface.hh"
#include "ogltexture.hh"

namespace ion {
namespace opengldrv {

	class OGLDevice;

	class ION_API OGL2DTexture:public video::Texture2D
	{
	public:
		class OGL2DSurface:public OGLSurface
		{
		public:
			OGL2DTexture *m_pOGL2DTexture;
			GLuint m_GLHandle,m_Framebuffer,m_DepthRenderbuffer,m_StencilRenderbuffer;
			GLint m_Level;

			void bind() const;
			void unbind() const;
		};

		OGL2DTexture(OGLDevice& ogldevice,const base::String& identifier,
			const ion_uint32 width,const ion_uint32 height,
			const ion_uint32 levels,const ion_uint32 flags,const video::Pixelformat format,const video::Memorypool pool);
		~OGL2DTexture();
	
		ion_uint8* pixeldata();
		const ion_uint8* pixeldata() const;

		//! Stride, in BYTES (not pixels)
		ion_uint32 stride() const;
		ion_uint32 bytesPerPixel() const;

		bool pixelLittleEndian() const;

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK();

		void* texHandle() const;
	
		ion_uint32 width() const;
		ion_uint32 height() const;
		ion_uint32 numMipmaplevels() const;

		void map();
		void unmap();

		video::Texture::Texsurface* surface(const ion_uint32 level);

		void currentLevel(const ion_uint32 level);
		ion_uint32 currentLevel() const;

		void copyFromFramebuffer(const ion_uint32 x,const ion_uint32 y);

		void glbind();
		GLuint handle() const;
		GLuint framebuffer() const;
		GLuint depthRenderbuffer() const;
		GLuint stencilRenderbuffer() const;

	protected:

		OGLDevice& m_rOGLDevice;
		bool m_IsValid,m_IsDataOK,m_IsMapped;
		GLuint m_GLHandle,m_Framebuffer,m_DepthRenderbuffer,m_StencilRenderbuffer;

		OGLTexture m_OGLTexture;
	
		GLint m_Internalformat;
		ion_uint32 m_Width,m_Height,m_LevelWidth,m_LevelHeight,m_NumMipmaplevels,m_MappedLevel,m_CurrentLevel;
		bool *m_pDataSubmitted;
		OGL2DSurface *m_pSurfaces;

		video::SimplePicbuffer *m_pPicbuffer;
	};

}
}

#endif
