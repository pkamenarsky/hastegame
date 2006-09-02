#ifndef ION_OPENGLDRV_OGLCUBEMAP_HH_INCLUDED
#define ION_OPENGLDRV_OGLCUBEMAP_HH_INCLUDED

#include <set>
#include <vector>
#include "iongl.hh"
#include "../base/dll.hh"
#include "../math/matrix.hh"
#include "../video/cubemap.hh"
#include "../video/mempool.hh"
#include "../video/pixelformat.hh"
#include "../video/simplepicbuffer.hh"

#include "oglsurface.hh"
#include "ogltexture.hh"

namespace ion {
namespace opengldrv {

	class OGLDevice;

	class ION_API OGLCubemap:public video::Cubemap
	{
	public:
		class OGLCubeSurface:public OGLSurface
		{
		public:
			OGLCubemap *m_pOGLCubemap;
			GLuint m_GLHandle,m_Framebuffer,m_DepthRenderbuffer,m_StencilRenderbuffer;
			GLint m_Level;
			GLenum m_Face;

			void bind() const;
			void unbind() const;
		};

		OGLCubemap(OGLDevice& ogldevice,const base::String& identifier,
			const ion_uint32 edgelength,const ion_uint32 levels,
			const ion_uint32 flags,const video::Pixelformat format,const video::Memorypool pool);
		~OGLCubemap();


		void currentFace(const Face face);
		Face currentFace() const;

		ion_uint32 numMipmaplevels() const;

		void map();
		void unmap();

		void currentLevel(const ion_uint32 level);
		ion_uint32 currentLevel() const;

		Texsurface* surface(const Face face,const ion_uint32 level);



		bool isDataOK() const;
		void dataIsOK();

		// A plugin-specific handle (for instance, in d3d9, this is a LPDIRECT3D.... pointer)
		void* texHandle() const;



		bool pixelLittleEndian() const;

		ion_uint8* pixeldata();
		const ion_uint8* pixeldata() const;

		//! Stride, in BYTES (not pixels)
		ion_uint32 stride() const;
		ion_uint32 bytesPerPixel() const;

		ion_uint32 width() const;
		ion_uint32 height() const;


		bool isValid() const;

		void glbind();
		GLuint handle() const;
	protected:
		GLenum tooglcubemapfaces(const Face face);

		OGLDevice& m_rOGLDevice;
		bool m_IsValid,m_IsDataOK,m_IsMapped;
		GLuint m_GLHandle,m_Framebuffer,m_DepthRenderbuffer,m_StencilRenderbuffer;

		OGLTexture m_OGLTexture;
	
		GLint m_Internalformat;
		ion_uint32 m_Edgelength,m_LevelEdgelength,m_NumMipmaplevels,m_MappedLevel,m_CurrentLevel;
		bool *m_pDataSubmitted;
		OGLCubeSurface *m_pSurfaces;

		video::SimplePicbuffer *m_pPicbuffer;

		Cubemap::Face m_CurrentFace;
	};

}
}

#endif
