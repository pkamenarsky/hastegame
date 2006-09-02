#include "tooglconvs.hh"

namespace ion {
namespace opengldrv {

	GLint oglpixelformat(const video::Pixelformat ionpixelformat)
	{
		switch (ionpixelformat) {
			case video::Pixelformat_RGB888:return GL_RGB8;
			case video::Pixelformat_XRGB8888:return GL_RGB8;
			case video::Pixelformat_XBGR8888:return GL_RGB8;
			case video::Pixelformat_ARGB8888:return GL_RGBA8;
			case video::Pixelformat_ABGR8888:return GL_RGBA8;
			case video::Pixelformat_BGR888:return GL_RGB8;
			case video::Pixelformat_RGBX8888:return GL_RGB8;
			case video::Pixelformat_BGRX8888:return GL_RGB8;
			case video::Pixelformat_RGBA8888:return GL_RGBA8;
			case video::Pixelformat_BGRA8888:return GL_RGBA8;
			case video::Pixelformat_RGB565:return GL_RGB5;
			case video::Pixelformat_XRGB1555:return GL_RGB5;
			case video::Pixelformat_ARGB1555:return GL_RGB5_A1;
			case video::Pixelformat_ARGB4444:return GL_RGBA4;
			case video::Pixelformat_RGB332:return GL_R3_G3_B2;
			case video::Pixelformat_XRGB4444:return GL_RGB4;
			case video::Pixelformat_A2RGB101010:return GL_RGB10_A2;
			case video::Pixelformat_A2BGR101010:return GL_RGB10_A2;
			case video::Pixelformat_ARGB16161616:return GL_RGBA16;

			/*case video::Pixelformat_FARGB16161616:return D3DFMT_A16B16G16R16F; // TODO: Rename FARGB to FABGR
			case video::Pixelformat_FARGB32323232:return D3DFMT_A32B32G32R32F; // TODO: Rename FARGB to FABGR
			case video::Pixelformat_D16:return D3DFMT_D16;
			case video::Pixelformat_D24:return D3DFMT_D24X8;
			case video::Pixelformat_D32:return D3DFMT_D32;
			case video::Pixelformat_D15S1:return D3DFMT_D15S1;
			case video::Pixelformat_D24S4:return D3DFMT_D24X4S4;
			case video::Pixelformat_D24S8:return D3DFMT_D24X8;
			case video::Pixelformat_FD24S8:return D3DFMT_D24FS8;
			case video::Pixelformat_RGB_DXT1:return D3DFMT_DXT1;
			case video::Pixelformat_RGBA_DXT1:return D3DFMT_DXT2; // TODO: Examine this one
			case video::Pixelformat_RGBA_DXT3:return D3DFMT_DXT3;
			case video::Pixelformat_RGBA_DXT5:return D3DFMT_DXT5;*/
			default:return -1;
		}
	}

	GLenum oglrgbaformat(const video::RGBALayouts rgbalayout)
	{
		switch (rgbalayout) {
			case video::RGBALayout_RGB:return GL_RGB;
			case video::RGBALayout_BGR:return GL_BGR;
			case video::RGBALayout_RGBA:return GL_RGBA;
			case video::RGBALayout_ARGB:return GL_RGBA;
			case video::RGBALayout_BGRA:return GL_BGRA;
			case video::RGBALayout_ABGR:return GL_BGRA;
			case video::RGBALayout_RGBX:return GL_RGB;
			case video::RGBALayout_XRGB:return GL_RGB;
			case video::RGBALayout_BGRX:return GL_BGR;
			case video::RGBALayout_XBGR:return GL_BGR;
			default:return GL_RGB;
		}
	}

	video::Pixelformat ogl2dtexformat(const video::Pixelformat format)
	{
		switch (format) {
			case video::Pixelformat_RGB888:
			case video::Pixelformat_XRGB8888:
			case video::Pixelformat_RGBX8888:
			case video::Pixelformat_RGB565:
			case video::Pixelformat_XRGB1555:
			case video::Pixelformat_RGB332:
			case video::Pixelformat_XRGB4444:
			case video::Pixelformat_RGB_DXT1:
			return video::Pixelformat_RGB888;

			case video::Pixelformat_BGR888:
			case video::Pixelformat_XBGR8888:
			case video::Pixelformat_BGRX8888:
			return video::Pixelformat_BGR888;

			case video::Pixelformat_ARGB8888:
			case video::Pixelformat_ARGB1555:
			case video::Pixelformat_ARGB4444:
			case video::Pixelformat_RGBA_DXT1:
			case video::Pixelformat_RGBA_DXT3:
			case video::Pixelformat_RGBA_DXT5:
			return video::Pixelformat_RGBA8888;

			case video::Pixelformat_ABGR8888:
			case video::Pixelformat_BGRA8888:
			return video::Pixelformat_BGRA8888;

			case video::Pixelformat_A2RGB101010:
			case video::Pixelformat_A2BGR101010:
			return video::Pixelformat_A2RGB101010;

			default:return format;
		}
	}

}
}
