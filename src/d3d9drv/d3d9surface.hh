#ifndef ION_D3D9DRV_D3D9SURFACE_HH_INCLUDED
#define ION_D3D9DRV_D3D9SURFACE_HH_INCLUDED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef DIRECT3D_VERSION
#define DIRECT3D_VERSION 0x0900
#endif

#include <windows.h>
#include <d3d9.h>

#include "../video/texture.hh"

namespace ion {
namespace d3d9drv {

	struct D3D9TextureSurface:public video::Texture::Texsurface
	{
		IDirect3DSurface9 *m_pD3D9Surface;

		D3D9TextureSurface() {}
	};
}
}

#endif
