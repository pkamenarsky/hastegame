#include <d3dx9.h>
#include <dxerr9.h>

#include "../base/log.hh"
#include "../video/pixelformat.hh"
#include "d3d9device.hh"
#include "d3d9texture2d.hh"
#include "tod3d9convs.hh"

namespace ion {
namespace d3d9drv {

	D3D9Texture2D::D3D9Texture2D(D3D9Device& d3d9device,const base::String&identifier,
		const ion_uint32 width,const ion_uint32 height,const ion_uint32 levels,const ion_uint32 flags,
		const video::Pixelformat format,const video::Memorypool pool):video::Texture2D(format,identifier,
		flags),m_pSurfaces(0),m_NumSurfaces(0),m_rD3D9Device(d3d9device),m_IsValid(true),m_IsDataOK(true),
		m_IsMapped(false),m_pTex(0),m_LevelWidth(0),m_LevelHeight(0),m_MappedLevel(0),m_CurrentLevel(0xffffffff)
	{
		UINT actualwidth=width,actualheight=height,actualnumlevels=levels;
		D3DFORMAT fmt=d3dpixelformat(format);
		D3DPOOL mempool=d3dmempool(pool);

		if (mempool!=D3DPOOL_DEFAULT) {
			if (flags&video::Textureflag_IsRendertaget) {
				base::log("D3D9Texture2D::D3D9Texture2D()",base::Warning) << "D3D render targets must be in a Mempool_Default pool. Using default mempool now.\n";
				mempool=D3DPOOL_DEFAULT; // NOTE: forcing DEFAULT
			} else if (flags&video::Textureflag_IsDynamic) {
				base::log("D3D9Texture2D::D3D9Texture2D()",base::Warning) << "D3D dynamic textures must be in a Mempool_Default pool. Using default mempool now.\n";
				mempool=D3DPOOL_DEFAULT; // NOTE: forcing DEFAULT
			}
		}

		if (fmt==D3DFMT_UNKNOWN) {
			m_IsValid=false;
			base::log("D3D9Texture2D::D3D9Texture2D()",base::Error) << "Unknown/unsupported pixel format\n";

			return;
		}

		DWORD usage=0;

		if (video::isDepthformat(format)) usage|=D3DUSAGE_DEPTHSTENCIL;
		if (flags&video::Textureflag_IsDisplacementMap) usage|=D3DUSAGE_DMAP;
		if (flags&video::Textureflag_IsDynamic) usage|=D3DUSAGE_DYNAMIC;
		if (flags&video::Textureflag_IsRendertaget) {
			if ((usage&D3DUSAGE_DEPTHSTENCIL)!=0)
				base::log("D3D9Texture2D::D3D9Texture2D()",base::Warning) << "D3D depth textures do not support the rendertarget flag. Omitting this flag now.\n";
			else
				usage|=D3DUSAGE_RENDERTARGET;
		}

		HRESULT hr=D3DXCheckTextureRequirements(
			d3d9device.d3d9deviceinterface(),&actualwidth,&actualheight,&actualnumlevels,
			(usage&D3DUSAGE_RENDERTARGET), // masks out everything else - this function only accepts this one flag
			&fmt,mempool);

		if (FAILED(hr)) {
			m_IsValid=false;
			const char *err=(const char*)(DXGetErrorDescription9(hr));
			base::log("D3D9Texture2D::D3D9Texture2D()",base::Error) << "D3DXCheckTextureRequirements() failed\n";
			base::logstream() << "DirectX error description: \"" << err << "\"\n";

			return;
		}

		hr=d3d9device.d3d9deviceinterface()->CreateTexture(
			actualwidth,actualheight,actualnumlevels,usage,fmt,mempool,&m_pTex,0);

		if (FAILED(hr)) {
			m_IsValid=false;
			const char *err=(const char*)(DXGetErrorDescription9(hr));
			base::log("D3D9Texture2D::D3D9Texture2D()",base::Error) << "CreateTexture() failed\n";
			base::logstream() << "DirectX error description: \"" << err << "\"\n";
		}

		m_Width=actualwidth;
		m_Height=actualheight;
		m_Pixelformat=fromd3dpixelformat(fmt);
		m_NumMipmaplevels=actualnumlevels;

		if (m_Pixelformat==video::Pixelformat_Unknown) {
			base::log("D3D9Texture2D::D3D9Texture2D()",base::Warning) << "Actual pixelformat unknown\n";
		}

		{
			m_NumSurfaces=m_pTex->GetLevelCount();
			m_pSurfaces=new D3D9TextureSurface[m_NumSurfaces];

			for (ion_uint32 ii=0;ii<m_NumSurfaces;++ii) {
				D3DSURFACE_DESC desc;
				m_pTex->GetLevelDesc(ii,&desc);

				m_pSurfaces[ii].m_pTexture=this;
				m_pSurfaces[ii].m_Width=desc.Width;
				m_pSurfaces[ii].m_Height=desc.Height;
				m_pTex->GetSurfaceLevel(ii,&(m_pSurfaces[ii].m_pD3D9Surface));
			}
		}

		currentLevel(0);
	}

	D3D9Texture2D::~D3D9Texture2D()
	{
		unmap();
		for (ion_uint32 ii=0;ii<m_NumSurfaces;++ii) {
			m_pSurfaces[ii].m_pD3D9Surface->Release();
		}
		if (m_pSurfaces) delete [] m_pSurfaces;
		if (m_pTex) m_pTex->Release();
	}

	ion_uint8* D3D9Texture2D::pixeldata()
	{
		return (ion_uint8*)(m_LockedRect.pBits);
	}

	const ion_uint8* D3D9Texture2D::pixeldata() const
	{
		return (ion_uint8*)(m_LockedRect.pBits);
	}

	ion_uint32 D3D9Texture2D::stride() const
	{
		return m_LockedRect.Pitch;
	}

	ion_uint32 D3D9Texture2D::bytesPerPixel() const
	{
		return pixelformatsize(m_Pixelformat);
	}

	bool D3D9Texture2D::pixelLittleEndian() const
	{
		return true;
	}

	bool D3D9Texture2D::isValid() const
	{
		return m_IsValid;
	}

	bool D3D9Texture2D::isDataOK() const
	{
		return m_IsDataOK;
	}

	void D3D9Texture2D::dataIsOK() { m_IsDataOK=true; }

	void* D3D9Texture2D::texHandle() const
	{
		return m_pTex;
	}

	ion_uint32 D3D9Texture2D::width() const
	{
		return m_LevelWidth;
	}

	ion_uint32 D3D9Texture2D::height() const
	{
		return m_LevelHeight;
	}

	ion_uint32 D3D9Texture2D::numMipmaplevels() const
	{
		return m_NumMipmaplevels;
	}

	void D3D9Texture2D::map()
	{
		if (!m_IsMapped) {
			m_pTex->LockRect(m_CurrentLevel,&m_LockedRect,0,0);
			m_MappedLevel=m_CurrentLevel;
			m_IsMapped=true;
		}
	}

	void D3D9Texture2D::unmap()
	{
		if (m_IsMapped) {
			m_pTex->UnlockRect(m_MappedLevel);
			m_IsMapped=false;
		}
	}

	video::Texture::Texsurface* D3D9Texture2D::surface(const ion_uint32 level)
	{
		return (level<m_NumSurfaces) ? &(m_pSurfaces[level]) : 0;
	}

	void D3D9Texture2D::currentLevel(const ion_uint32 level)
	{
		if ((m_CurrentLevel==level) || m_IsMapped) return;

		m_CurrentLevel=level;

		D3DSURFACE_DESC desc;
		m_pTex->GetLevelDesc(level,&desc);

		m_LevelWidth=desc.Width;
		m_LevelHeight=desc.Height;
	}

	ion_uint32 D3D9Texture2D::currentLevel() const
	{
		return m_CurrentLevel;
	}

	void D3D9Texture2D::copyFromFramebuffer(const ion_uint32 x,const ion_uint32 y)
	{
	}

	LPDIRECT3DTEXTURE9 D3D9Texture2D::d3d9texture() const
	{
		return m_pTex;
	}

	void D3D9Texture2D::d3d9releaseBeforeReset()
	{
		m_IsDataOK=false;
	}

	void D3D9Texture2D::d3d9restoreAfterReset()
	{
		m_IsDataOK=false;
	}

}
}
