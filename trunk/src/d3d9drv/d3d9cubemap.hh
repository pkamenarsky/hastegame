#ifndef ION_D3D9DRV_D3D9CUBEMAP_HH_INCLUDED
#define ION_D3D9DRV_D3D9CUBEMAP_HH_INCLUDED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef DIRECT3D_VERSION
#define DIRECT3D_VERSION 0x0900
#endif

#include <windows.h>
#include <d3d9.h>

#include "../video/mempool.hh"
#include "../video/pixelformat.hh"
#include "../video/cubemap.hh"
#include "d3d9surface.hh"

namespace ion {
namespace d3d9drv {

	class D3D9Device;

	class ION_API D3D9Cubemap:public video::Cubemap
	{
	public:
		D3D9Cubemap(D3D9Device& d3d9device,const base::String& identifier,
			const ion_uint32 edgelength,const ion_uint32 levels,
			const ion_uint32 flags,const video::Pixelformat format,const video::Memorypool pool);
		~D3D9Cubemap();

		void currentFace(const Cubemap::Face face);
		Cubemap::Face currentFace() const;

		ion_uint32 numMipmaplevels() const;

		void map();
		void unmap();

		void currentLevel(const ion_uint32 level);
		ion_uint32 currentLevel() const;

		video::Texture::Texsurface* surface(const Face face,const ion_uint32 level);

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK();

		void* texHandle() const;

		bool pixelLittleEndian() const;

		ion_uint8* pixeldata();
		const ion_uint8* pixeldata() const;

		//! Stride, in BYTES (not pixels)
		ion_uint32 stride() const;
		ion_uint32 bytesPerPixel() const;

		ion_uint32 width() const;
		ion_uint32 height() const;

		LPDIRECT3DCUBETEXTURE9 d3d9cubetexture() const;
		void d3d9releaseBeforeReset();
		void d3d9restoreAfterReset();

	protected:
		D3DCUBEMAP_FACES tod3dcubemapfaces(const Face face);

		D3D9TextureSurface **m_pSurfaces;
		D3D9Device& m_rD3D9Device;
		LPDIRECT3DCUBETEXTURE9 m_pCubemap;
		D3DLOCKED_RECT m_LockedRect;
		bool m_IsValid,m_IsDataOK,m_IsMapped;
		ion_uint32 m_Width,m_Height,m_LevelWidth,m_LevelHeight,m_NumMipmaplevels,m_MappedLevel,m_CurrentLevel;
		Cubemap::Face m_CurrentFace;
	};

}
}

#endif
