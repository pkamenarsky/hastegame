#include "texture.hh"

namespace ion {
namespace video {

	Texture::Texsurface::Texsurface()
	{
	}

	Texture::Texsurface::~Texsurface()
	{
	}

	Pixelformat Texture::requestedpixelformat() const { return m_RequestedPixelformat; }

	Texture::Texture(const Pixelformat requestedpixelformat,const base::String& identifier):
	RefCounted(identifier.cstr()),m_RequestedPixelformat(requestedpixelformat) {}

}
}
