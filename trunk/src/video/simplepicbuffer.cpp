#include "simplepicbuffer.hh"

namespace ion {
namespace video {

	SimplePicbuffer::SimplePicbuffer(const ion_uint32 width,const ion_uint32 height,const Pixelformat pixelformat):
	MemPicbuffer(pixelformat),m_Width(width),m_Height(height)
	{
		m_Datablock.resize(stride()*height);
	}

	SimplePicbuffer::SimplePicbuffer(const Picbuffer& src):
	MemPicbuffer(src.pixelformat()),m_Width(src.width()),m_Height(src.height())
	{
		m_Datablock.resize(stride()*m_Height);
		transfer(src);
	}

	SimplePicbuffer::~SimplePicbuffer()
	{
	}

	void SimplePicbuffer::resizeDimensions(const ion_uint32 width,const ion_uint32 height)
	{
		if ((m_Width==width) && (m_Height==height)) return;

		m_Width=width;
		m_Height=height;
		m_Datablock.resize(stride()*height);
	}

	ion_uint32 SimplePicbuffer::bytesPerPixel() const
	{
		return pixelformatsize(m_Pixelformat);
	}

}
}
