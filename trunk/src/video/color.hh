#ifndef ION_VIDEO_COLOR_HH_INCLUDED
#define ION_VIDEO_COLOR_HH_INCLUDED

#include "../base/iontypes.hh"
#include "../base/numericarrays.hh"

namespace ion {
namespace video {

	class ION_API Color:public base::Float4
	{
	public:
		static Color& white();
		static Color& black();
		static Color& red();
		static Color& darkred();
		static Color& green();
		static Color& darkgreen();
		static Color& blue();
		static Color& darkblue();
		static Color& yellow();
		static Color& orange();
		static Color& darkgray();
		static Color& lightgray();


		Color() {}
		Color(const float r,const float g,const float b,const float a);
		Color(const float *f);
		Color(const base::Float3& float3);
		Color(const base::Float4& float4);
		Color(const Color& src);

		float r() const;
		float g() const;
		float b() const;
		float a() const;

		ion_uint8 rUint8() const;
		ion_uint8 gUint8() const;
		ion_uint8 bUint8() const;
		ion_uint8 aUint8() const;

		void r(const float newr);
		void g(const float newg);
		void b(const float newb);
		void a(const float newa);

		// MSB    A | R | G | B    LSB
		ion_uint32 argb32Bit() const;
		void argb32Bit(const ion_uint32 argb);

		void desaturate();
		void desaturate(const float rweight,const float gweight,const float bweight);
	};

}
}

#endif
