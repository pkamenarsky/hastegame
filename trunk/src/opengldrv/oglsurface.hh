#ifndef ION_OPENGLDRV_OGLSURFACE_HH_INCLUDED
#define ION_OPENGLDRV_OGLSURFACE_HH_INCLUDED

#include "../video/texture.hh"
#include "ogltexture.hh"

namespace ion {
namespace opengldrv {

	class OGLSurface:public video::Texture::Texsurface
	{
	public:
		virtual void bind() const=0;
		virtual void unbind() const=0;
	};

}
}

#endif
