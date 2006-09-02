#ifndef ION_OPENGLDRV_RENDERTOTEXTURE_HH_INCLUDED
#define ION_OPENGLDRV_RENDERTOTEXTURE_HH_INCLUDED

#include "iongl.hh"
#include "ogl2dtexture.hh"

namespace ion {
namespace opengldrv {

	//! Base class for render-to-texture capability in OpenGL.

	class OGLRenderToTexture
	{
	public:
		virtual ~OGLRenderToTexture() {}

		// NOTE: index is for MRTs
		void set(const ion_uint32 index,OGL2DTexture* pTex);
	protected:
		OGLRenderToTexture() {}
	};

}
}

#endif
