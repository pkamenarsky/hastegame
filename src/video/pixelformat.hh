#ifndef ION_VIDEO_PIXELFORMAT_HH_INCLUDED
#define ION_VIDEO_PIXELFORMAT_HH_INCLUDED

#include "../base/dll.hh"
#include "channeldatatype.hh"

namespace ion {
namespace video {

	enum RGBALayouts {
		RGBALayout_RGB,
		RGBALayout_BGR,

		RGBALayout_RGBA,
		RGBALayout_ARGB,
		RGBALayout_BGRA,
		RGBALayout_ABGR,

		RGBALayout_RGBX,
		RGBALayout_XRGB,
		RGBALayout_BGRX,
		RGBALayout_XBGR,

		RGBALayout_Invalid
	};

	enum Pixelformat {
		Pixelformat_RGB888=0,			// 24-bit RGB
		Pixelformat_BGR888=1,			// 24-bit BGR
		Pixelformat_XRGB8888=2,			// 32-bit RGB (the upper 8 bit are ignored)
		Pixelformat_XBGR8888=3,			// 32-bit BGR (the upper 8 bit are ignored)
		Pixelformat_ARGB8888=4,			// 32-bit ARGB
		Pixelformat_ABGR8888=5,			// 32-bit ABGR
		Pixelformat_RGBX8888=6,			// 32-bit RGB (the lower 8 bit are ignored)
		Pixelformat_BGRX8888=7,			// 32-bit BGR (the lower 8 bit are ignored)
		Pixelformat_RGBA8888=8,			// 32-bit RGBA
		Pixelformat_BGRA8888=9,			// 32-bit BGRA
		Pixelformat_RGB565=10,			// 16-bit RGB (R 5 bit G 6 bit B 5 bit)
		Pixelformat_XRGB1555=11,		// 15-bit RGB (5 bit per component, the MSB is ignored)
		Pixelformat_ARGB1555=12,		// 15-bit ARGB (5 bit per RGB component, the MSB is used for alpha)
		Pixelformat_ARGB4444=13,		// 16-bit ARGB (4 bit per component)
		Pixelformat_RGB332=14,
		Pixelformat_XRGB4444=15,
		Pixelformat_A2RGB101010=16,
		Pixelformat_A2BGR101010=17,
		Pixelformat_ARGB16161616=18,
		Pixelformat_FARGB16161616=19,
		Pixelformat_FARGB32323232=20,

		Pixelformat_D16=21,
		Pixelformat_D24=22,
		Pixelformat_D32=23,
		Pixelformat_D15S1=24,
		Pixelformat_D24S4=25,
		Pixelformat_D24S8=26,
		Pixelformat_FD24S8=27,

		Pixelformat_RGB_DXT1=28,
		Pixelformat_RGBA_DXT1=29,
		Pixelformat_RGBA_DXT3=30,
		Pixelformat_RGBA_DXT5=31,

		Pixelformat_FR32=32,
		Pixelformat_A8=33,
		Pixelformat_L8=34,
		Pixelformat_L16=35,
		Pixelformat_A4L4=36,
		Pixelformat_A8L8=37,

		Pixelformat_NumFormats=38,

		Pixelformat_Unknown=0xfffe,
		Pixelformat_None=0xffff
	};

	// @return true if pixelformat is a depth format
	ION_API bool isDepthformat(const Pixelformat pixelformat);

	// @return true if pixelformat has an alpha channel
	ION_API bool pixelformatHasAlphachannel(const Pixelformat pixelformat);

	// @return true if pixelformat is a color format (includes floating point color formats)
	ION_API bool isColorformat(const Pixelformat pixelformat);

	// Returns true if at least one channel of the colorformat has more than 8 bit
	ION_API bool colorformatGreaterThan8Bits(const Pixelformat pixelformat);

	// @return true if pixelformat is a floating-point color format
	ION_API bool isFloatColorformat(const Pixelformat pixelformat);

	ION_API RGBALayouts rgbalayoutFromPixelformat(const Pixelformat pixelformat);

	ION_API Pixelformat suggestPixelformat(const RGBALayouts rgbalayout,const Channeldatatype chtype,
		RGBALayouts& actualrgbalayout);

	ION_API unsigned int numRGBALayoutChannels(const RGBALayouts fmt);

	ION_API unsigned int numRGBALayoutComponents(const RGBALayouts fmt);

	ION_API unsigned int pixelformatsize(const Pixelformat pixelformat);

}
}

#endif
