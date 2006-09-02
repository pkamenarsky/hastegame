#ifndef ION_OPENGLDRV_OPENGLCONVERSIONS_HH_INCLUDED
#define ION_OPENGLDRV_OPENGLCONVERSIONS_HH_INCLUDED

#include "iongl.hh"

#include "../base/numericarrays.hh"
#include "../video/coreenums.hh"
#include "../video/mempool.hh"
#include "../video/pixelformat.hh"

namespace ion {
namespace opengldrv {

	GLint oglpixelformat(const video::Pixelformat ionpixelformat);
	GLenum oglrgbaformat(const video::RGBALayouts rgbalayout);
	video::Pixelformat ogl2dtexformat(const video::Pixelformat format);
}
}

#endif
