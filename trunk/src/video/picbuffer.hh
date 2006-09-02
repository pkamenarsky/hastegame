#ifndef ION_VIDEO_PICBUFFER_HH_INCLUDED
#define ION_VIDEO_PICBUFFER_HH_INCLUDED

#include "../base/datablock.hh"
#include "../base/iontypes.hh"
#include "channeldatatype.hh"
#include "pixelformat.hh"

namespace ion {
namespace video {

	class ION_API Picbuffer
	{
	public:
		virtual ~Picbuffer() {}

		inline const Pixelformat& pixelformat() const { return m_Pixelformat; }
		virtual bool pixelLittleEndian() const=0;

		virtual ion_uint8* pixeldata()=0;
		virtual const ion_uint8* pixeldata() const=0;

		//! Stride, in BYTES (not pixels)
		virtual ion_uint32 stride() const=0;
		virtual ion_uint32 bytesPerPixel() const=0;

		virtual ion_uint32 width() const=0;
		virtual ion_uint32 height() const=0;

		float getpixeldepth(const ion_uint32 x,const ion_uint32 y) const;
		void putpixeldepth(const ion_uint32 x,const ion_uint32 y,const float depth);

		void getpixelUint8(const ion_uint32 x,const ion_uint32 y,ion_uint8 &r,ion_uint8 &g,ion_uint8 &b,ion_uint8 &a) const;
		void getpixelUint16(const ion_uint32 x,const ion_uint32 y,ion_uint16 &r,ion_uint16 &g,ion_uint16 &b,ion_uint16 &a) const;
		void getpixelFloat(const ion_uint32 x,const ion_uint32 y,float &r,float &g,float &b,float &a) const;

		void putpixelUint8(const ion_uint32 x,const ion_uint32 y,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b,const ion_uint8 a);
		void putpixelUint16(const ion_uint32 x,const ion_uint32 y,const ion_uint16 r,const ion_uint16 g,const ion_uint16 b,const ion_uint16 a);
		void putpixelFloat(const ion_uint32 x,const ion_uint32 y,const float r,const float g,const float b,const float a);

		void clear(const ion_uint8 r,const ion_uint8 g,const ion_uint8 b,const ion_uint8 a);
		void transfer(const Picbuffer& src);
	protected:
		Picbuffer(const Pixelformat pixelformat):m_Pixelformat(pixelformat) {}

		Pixelformat m_Pixelformat;
	};

	class ION_API MemPicbuffer:public Picbuffer
	{
	public:
		MemPicbuffer(const Pixelformat pixelformat);
		MemPicbuffer(const MemPicbuffer& memPicbuffer);
		~MemPicbuffer();

		ion_uint8* pixeldata();
		const ion_uint8* pixeldata() const;

		const base::Datablock& datablock() const;
		base::Datablock& datablock();

	protected:
		base::Datablock m_Datablock;
	};

	//ION_API void transfer(const Picbuffer& src,Picbuffer& dest);

}
}

#endif
