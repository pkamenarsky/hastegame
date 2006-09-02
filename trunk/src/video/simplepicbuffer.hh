#ifndef ION_VIDEO_SIMPLEPICBUFFER_HH_INCLUDED
#define ION_VIDEO_SIMPLEPICBUFFER_HH_INCLUDED

#include "../base/datablock.hh"
#include "../base/iontypes.hh"
#include "channeldatatype.hh"
#include "pixelformat.hh"
#include "picbuffer.hh"

namespace ion {
namespace video {

	class ION_API SimplePicbuffer:public MemPicbuffer
	{
	public:
		SimplePicbuffer(const ion_uint32 width,const ion_uint32 height,const Pixelformat pixelformat);
		SimplePicbuffer(const Picbuffer& src);
		~SimplePicbuffer();

		void resizeDimensions(const ion_uint32 width,const ion_uint32 height);

		inline ion_uint32 stride() const { return m_Width*bytesPerPixel(); }
		ion_uint32 bytesPerPixel() const;

		inline ion_uint32 width() const { return m_Width; }
		inline ion_uint32 height() const { return m_Height; }

		inline bool pixelLittleEndian() const { return false; }

	protected:
		ion_uint32 m_Width,m_Height;
	};

}
}

#endif
