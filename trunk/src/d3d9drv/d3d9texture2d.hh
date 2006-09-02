#ifndef ION_D3D9DRV_D3D9TEXTURE2D_HH_INCLUDED
#define ION_D3D9DRV_D3D9TEXTURE2D_HH_INCLUDED

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
#include "../video/texture2d.hh"
#include "d3d9surface.hh"

namespace ion {
namespace d3d9drv {

	class D3D9Device;

	class ION_API D3D9Texture2D:public video::Texture2D
	{
	public:
		D3D9Texture2D(D3D9Device& d3d9device,const base::String&identifier,
			const ion_uint32 width,const ion_uint32 height,const ion_uint32 levels,
			const ion_uint32 flags,const video::Pixelformat format,const video::Memorypool pool);
		~D3D9Texture2D();

		ion_uint8* pixeldata();
		const ion_uint8* pixeldata() const;

		//! Stride, in BYTES (not pixels)
		ion_uint32 stride() const;
		ion_uint32 bytesPerPixel() const;

		bool pixelLittleEndian() const;

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK();

		void* texHandle() const;

		ion_uint32 width() const;
		ion_uint32 height() const;
		ion_uint32 numMipmaplevels() const;

		void map();
		void unmap();

		video::Texture::Texsurface* surface(const ion_uint32 level);

		void currentLevel(const ion_uint32 level);
		ion_uint32 currentLevel() const;

		void copyFromFramebuffer(const ion_uint32 x,const ion_uint32 y);

		LPDIRECT3DTEXTURE9 d3d9texture() const;
		void d3d9releaseBeforeReset();
		void d3d9restoreAfterReset();

	protected:

		D3D9TextureSurface *m_pSurfaces;
		ion_uint32 m_NumSurfaces;
		D3D9Device& m_rD3D9Device;
		bool m_IsValid,m_IsDataOK,m_IsMapped;
		LPDIRECT3DTEXTURE9 m_pTex;
		D3DLOCKED_RECT m_LockedRect;

//		D3DFORMAT m_Internalformat;
		ion_uint32 m_Width,m_Height,m_LevelWidth,m_LevelHeight,m_NumMipmaplevels,m_MappedLevel,m_CurrentLevel;
	};

}
}

#endif
