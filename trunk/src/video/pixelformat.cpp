#include "pixelformat.hh"

namespace ion {
namespace video {

	bool isDepthformat(const Pixelformat pixelformat)
	{
		switch (pixelformat) {
			case Pixelformat_D16:
			case Pixelformat_D24:
			case Pixelformat_D32:
			case Pixelformat_D15S1:
			case Pixelformat_D24S4:
			case Pixelformat_D24S8:
			case Pixelformat_FD24S8:
				return true;
			default:return false;
		}
	}

	bool isColorformat(const Pixelformat pixelformat)
	{
		switch (pixelformat) {
			case Pixelformat_RGB888:
			case Pixelformat_BGR888:
			case Pixelformat_XRGB8888:
			case Pixelformat_XBGR8888:
			case Pixelformat_ARGB8888:
			case Pixelformat_ABGR8888:
			case Pixelformat_RGBX8888:
			case Pixelformat_BGRX8888:
			case Pixelformat_RGBA8888:
			case Pixelformat_BGRA8888:
			case Pixelformat_RGB565:
			case Pixelformat_XRGB1555:
			case Pixelformat_ARGB1555:
			case Pixelformat_ARGB4444:
			case Pixelformat_RGB332:
			case Pixelformat_XRGB4444:
			case Pixelformat_A2RGB101010:
			case Pixelformat_A2BGR101010:
			case Pixelformat_ARGB16161616:
			case Pixelformat_FARGB16161616:
			case Pixelformat_FARGB32323232:
			case Pixelformat_FR32:
				return true;
			default:return false;
		}
	}

	bool pixelformatHasAlphachannel(const Pixelformat pixelformat)
	{
		switch (pixelformat) {
			case Pixelformat_ARGB8888:
			case Pixelformat_ABGR8888:
			case Pixelformat_RGBA8888:
			case Pixelformat_BGRA8888:
			case Pixelformat_ARGB1555:
			case Pixelformat_ARGB4444:
			case Pixelformat_A2RGB101010:
			case Pixelformat_A2BGR101010:
			case Pixelformat_ARGB16161616:
			case Pixelformat_FARGB16161616:
			case Pixelformat_FARGB32323232:
			case Pixelformat_A8:
			case Pixelformat_A4L4:
			case Pixelformat_A8L8:
				return true;
			default:return false;
		}
	}

	unsigned int numRGBALayoutChannels(const RGBALayouts fmt)
	{
		switch (fmt) {
			case RGBALayout_RGB:return 3;
			case RGBALayout_BGR:return 3;
			case RGBALayout_ARGB:return 4;
			case RGBALayout_ABGR:return 4;
			case RGBALayout_RGBA:return 4;
			case RGBALayout_BGRA:return 4;
			case RGBALayout_XRGB:return 3;
			case RGBALayout_XBGR:return 3;
			case RGBALayout_RGBX:return 3;
			case RGBALayout_BGRX:return 3;
			default: break;
		}

		return 0;
	}

	unsigned int numRGBALayoutComponents(const RGBALayouts fmt)
	{
		switch (fmt) {
			case RGBALayout_RGB:return 3;
			case RGBALayout_BGR:return 3;
			case RGBALayout_ARGB:return 4;
			case RGBALayout_ABGR:return 4;
			case RGBALayout_RGBA:return 4;
			case RGBALayout_BGRA:return 4;
			case RGBALayout_XRGB:return 4;
			case RGBALayout_XBGR:return 4;
			case RGBALayout_RGBX:return 4;
			case RGBALayout_BGRX:return 4;
			default: break;
		}

		return 0;
	}

	bool colorformatGreaterThan8Bits(const Pixelformat pixelformat)
	{
		switch (pixelformat) {
			case Pixelformat_A2RGB101010:
			case Pixelformat_A2BGR101010:
			case Pixelformat_ARGB16161616:
			case Pixelformat_FARGB16161616:
			case Pixelformat_FARGB32323232:
				return true;
			default:return false;
		}
	}

	bool isFloatColorformat(const Pixelformat pixelformat)
	{
		switch (pixelformat) {
			case Pixelformat_FARGB16161616:
			case Pixelformat_FARGB32323232:
				return true;
			default:return false;
		}
	}

	RGBALayouts rgbalayoutFromPixelformat(const Pixelformat pixelformat)
	{
		switch (pixelformat) {
			case Pixelformat_RGB888:return RGBALayout_RGB;
			case Pixelformat_BGR888:return RGBALayout_BGR;
			case Pixelformat_XRGB8888:return RGBALayout_XRGB;
			case Pixelformat_XBGR8888:return RGBALayout_XBGR;
			case Pixelformat_ARGB8888:return RGBALayout_ARGB;
			case Pixelformat_ABGR8888:return RGBALayout_ABGR;
			case Pixelformat_RGBX8888:return RGBALayout_XRGB;
			case Pixelformat_BGRX8888:return RGBALayout_XBGR;
			case Pixelformat_RGBA8888:return RGBALayout_ARGB;
			case Pixelformat_BGRA8888:return RGBALayout_ABGR;
			case Pixelformat_RGB565:return RGBALayout_RGB;
			case Pixelformat_XRGB1555:return RGBALayout_XRGB;
			case Pixelformat_ARGB1555:return RGBALayout_ARGB;
			case Pixelformat_ARGB4444:return RGBALayout_ARGB;
			case Pixelformat_RGB332:return RGBALayout_RGB;
			case Pixelformat_XRGB4444:return RGBALayout_XRGB;
			case Pixelformat_A2RGB101010:return RGBALayout_ARGB;
			case Pixelformat_A2BGR101010:return RGBALayout_ABGR;
			case Pixelformat_ARGB16161616:return RGBALayout_ARGB;
			case Pixelformat_FARGB16161616:return RGBALayout_ARGB;
			case Pixelformat_FARGB32323232:return RGBALayout_ARGB;
			default:return RGBALayout_Invalid;
		}
	}

	unsigned int pixelformatsize(const Pixelformat pixelformat)
	{
		switch (pixelformat) {
			case Pixelformat_RGB888:return 3;
			case Pixelformat_BGR888:return 3;
			case Pixelformat_XRGB8888:return 4;
			case Pixelformat_XBGR8888:return 4;
			case Pixelformat_ARGB8888:return 4;
			case Pixelformat_ABGR8888:return 4;
			case Pixelformat_RGBX8888:return 4;
			case Pixelformat_BGRX8888:return 4;
			case Pixelformat_RGBA8888:return 4;
			case Pixelformat_BGRA8888:return 4;
			case Pixelformat_RGB565:return 2;
			case Pixelformat_XRGB1555:return 2;
			case Pixelformat_ARGB1555:return 2;
			case Pixelformat_ARGB4444:return 2;
			case Pixelformat_RGB332:return 1;
			case Pixelformat_XRGB4444:return 2;
			case Pixelformat_A2RGB101010:return 4;
			case Pixelformat_A2BGR101010:return 4;
			case Pixelformat_ARGB16161616:return 8;
			case Pixelformat_FARGB16161616:return 8;
			case Pixelformat_FARGB32323232:return 16;
			case Pixelformat_FR32:return 4;
			case Pixelformat_A8:return 1;
			case Pixelformat_L8:return 1;
			case Pixelformat_L16:return 2;
			case Pixelformat_A4L4:return 1;
			case Pixelformat_A8L8:return 2;
			default:return 0;
		}
	}

	Pixelformat suggestPixelformat(const RGBALayouts rgbalayout,const Channeldatatype chtype,
		RGBALayouts& actualrgbalayout)
	{
		switch (chtype) {
			case Channeldatatype_Uint8:
				switch (rgbalayout) {
					case RGBALayout_XRGB:return Pixelformat_XRGB8888; actualrgbalayout=RGBALayout_XRGB; break;
					case RGBALayout_XBGR:return Pixelformat_XBGR8888; actualrgbalayout=RGBALayout_XBGR; break;
					case RGBALayout_ARGB:return Pixelformat_ARGB8888; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_ABGR:return Pixelformat_ABGR8888; actualrgbalayout=RGBALayout_ABGR; break;
					case RGBALayout_RGBA:return Pixelformat_RGBA8888; actualrgbalayout=RGBALayout_RGBA; break;
					case RGBALayout_BGRA:return Pixelformat_BGRA8888; actualrgbalayout=RGBALayout_BGRA; break;
					default:return Pixelformat_Unknown; break;
				} break;
			case Channeldatatype_Uint16:
				switch (rgbalayout) {
					case RGBALayout_ARGB:return Pixelformat_ARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_ABGR:return Pixelformat_ARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_RGBA:return Pixelformat_ARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_BGRA:return Pixelformat_ARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					default:return Pixelformat_Unknown; break;
				} break;
			case Channeldatatype_Float16:
				switch (rgbalayout) {
					case RGBALayout_ARGB:return Pixelformat_FARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_ABGR:return Pixelformat_FARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_RGBA:return Pixelformat_FARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_BGRA:return Pixelformat_FARGB16161616; actualrgbalayout=RGBALayout_ARGB; break;
					default:return Pixelformat_Unknown; break;
				} break;
			case Channeldatatype_Float32:
				switch (rgbalayout) {
					case RGBALayout_ARGB:return Pixelformat_FARGB32323232; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_ABGR:return Pixelformat_FARGB32323232; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_RGBA:return Pixelformat_FARGB32323232; actualrgbalayout=RGBALayout_ARGB; break;
					case RGBALayout_BGRA:return Pixelformat_FARGB32323232; actualrgbalayout=RGBALayout_ARGB; break;
					default:return Pixelformat_Unknown; break;
				} break;
			default: return Pixelformat_Unknown;
		}

		return Pixelformat_Unknown;
	}

}
}
