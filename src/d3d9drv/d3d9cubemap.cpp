#include <d3dx9.h>
#include <dxerr9.h>

#include "../base/log.hh"
#include "d3d9device.hh"
#include "d3d9cubemap.hh"
#include "tod3d9convs.hh"

namespace ion {
namespace d3d9drv {

	D3D9Cubemap::D3D9Cubemap(D3D9Device& d3d9device,const base::String&identifier,
		const ion_uint32 edgelength,const ion_uint32 levels,
		const ion_uint32 flags,const video::Pixelformat format,const video::Memorypool pool):
		Cubemap(format,identifier),m_pSurfaces(0),m_rD3D9Device(d3d9device),m_pCubemap(0),m_IsValid(true),m_IsDataOK(true),
		m_IsMapped(false),m_Width(edgelength),m_Height(edgelength),m_LevelWidth(0),m_LevelHeight(0),m_MappedLevel(0),
		m_CurrentLevel(0xffffffff),m_CurrentFace(FrontFace)
	{
		UINT actualedgelength=edgelength,actualnumlevels=levels;
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
			base::log("D3D9Cubemap()",base::Error) << "Unknown/unsupported pixel format\n";

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

		HRESULT hr=D3DXCheckCubeTextureRequirements(
			d3d9device.d3d9deviceinterface(),&actualedgelength,&actualnumlevels,
			(usage&D3DUSAGE_RENDERTARGET), // masks out everything else - this function only accepts this one flag
			&fmt,mempool);

		if (FAILED(hr)) {
			m_IsValid=false;
			const char *err=(const char*)(DXGetErrorDescription9(hr));
			base::log("D3D9Cubemap()",base::Error) << "D3DXCheckCubeTextureRequirements() failed\n";
			base::logstream() << "DirectX error description: \"" << err << "\"\n";

			return;
		}

		hr=d3d9device.d3d9deviceinterface()->CreateCubeTexture(
			actualedgelength,actualnumlevels,usage,fmt,mempool,&m_pCubemap,0);

		if (FAILED(hr)) {
			m_IsValid=false;
			const char *err=(const char*)(DXGetErrorDescription9(hr));
			base::log("D3D9Cubemap()",base::Error) << "CreateCubeTexture() failed\n";
			base::logstream() << "DirectX error description: \"" << err << "\"\n";
		}

		m_Width=actualedgelength;
		m_Height=actualedgelength;
		m_Pixelformat=fromd3dpixelformat(fmt);
		m_NumMipmaplevels=actualnumlevels;

		if (m_Pixelformat==video::Pixelformat_Unknown) {
			base::log("D3D9Cubemap()",base::Warning) << "Actual pixelformat unknown\n";
		}

		m_pSurfaces=new D3D9TextureSurface*[6];
		for (unsigned int i=0;i<6;++i) {
			m_pSurfaces[i]=new D3D9TextureSurface[m_NumMipmaplevels];
			for (unsigned int j=0;j<m_NumMipmaplevels;++j) {
				D3DSURFACE_DESC desc;
				m_pCubemap->GetLevelDesc(j,&desc);

				Cubemap::Face face=(Cubemap::Face)i;
				m_pSurfaces[i][j].m_pTexture=this;
				m_pSurfaces[i][j].m_Width=desc.Width;
				m_pSurfaces[i][j].m_Height=desc.Height;
				m_pCubemap->GetCubeMapSurface(tod3dcubemapfaces(face),j,
					&(m_pSurfaces[i][j].m_pD3D9Surface));
			}
		}

		currentLevel(0);
	}

	D3D9Cubemap::~D3D9Cubemap()
	{
		if (m_pSurfaces) {
			for (unsigned int i=0;i<6;++i) {
				if (m_pSurfaces[i]) {
					for (unsigned int j=0;j<m_NumMipmaplevels;++j) {
						m_pSurfaces[i][j].m_pD3D9Surface->Release();
					}
					delete [] m_pSurfaces[i];
				}
			}
			delete [] m_pSurfaces;
		}
		if (m_pCubemap) m_pCubemap->Release();
	}

	D3DCUBEMAP_FACES D3D9Cubemap::tod3dcubemapfaces(const Face face)
	{
		switch (face) {
			case Cubemap::FrontFace:return D3DCUBEMAP_FACE_POSITIVE_Z;
			case Cubemap::BackFace:return D3DCUBEMAP_FACE_NEGATIVE_Z;
			case Cubemap::LeftFace:return D3DCUBEMAP_FACE_NEGATIVE_X;
			case Cubemap::RightFace:return D3DCUBEMAP_FACE_POSITIVE_X;
			case Cubemap::TopFace:return D3DCUBEMAP_FACE_POSITIVE_Y;
			case Cubemap::BottomFace:return D3DCUBEMAP_FACE_NEGATIVE_Y;
			default:return D3DCUBEMAP_FACE_POSITIVE_Z;
		}
	}

	void D3D9Cubemap::currentFace(const Face face)
	{
		m_CurrentFace=face;
	}

	D3D9Cubemap::Face D3D9Cubemap::currentFace() const
	{
		return m_CurrentFace;
	}

	ion_uint32 D3D9Cubemap::numMipmaplevels() const
	{
		return m_NumMipmaplevels;
	}

	void D3D9Cubemap::map()
	{
		if (!m_IsMapped) {
			m_pCubemap->LockRect(tod3dcubemapfaces(m_CurrentFace),m_CurrentLevel,&m_LockedRect,0,0);
			m_MappedLevel=m_CurrentLevel;
			m_IsMapped=true;
		}
	}

	void D3D9Cubemap::unmap()
	{
		if (m_IsMapped) {
			m_pCubemap->UnlockRect(tod3dcubemapfaces(m_CurrentFace),m_MappedLevel);
			m_IsMapped=false;
		}
	}

	void D3D9Cubemap::currentLevel(const ion_uint32 level)
	{
		if ((m_CurrentLevel==level) || m_IsMapped) return;

		m_CurrentLevel=level;

		D3DSURFACE_DESC desc;
		m_pCubemap->GetLevelDesc(level,&desc);

		m_LevelWidth=desc.Width;
		m_LevelHeight=desc.Height;
	}

	ion_uint32 D3D9Cubemap::currentLevel() const
	{
		return m_CurrentLevel;
	}

	video::Texture::Texsurface* D3D9Cubemap::surface(const Face face,const ion_uint32 level)
	{
		return (level<m_NumMipmaplevels) ? &(m_pSurfaces[face][level]) : 0;
	}

	bool D3D9Cubemap::isValid() const
	{
		return m_IsValid;
	}

	bool D3D9Cubemap::isDataOK() const
	{
		return m_IsDataOK;
	}

	void D3D9Cubemap::dataIsOK()
	{
		m_IsDataOK=true;
	}

	void* D3D9Cubemap::texHandle() const
	{
		return m_pCubemap;
	}

	bool D3D9Cubemap::pixelLittleEndian() const
	{
		return true;
	}

	ion_uint8* D3D9Cubemap::pixeldata()
	{
		return (ion_uint8*)(m_LockedRect.pBits);
	}

	const ion_uint8* D3D9Cubemap::pixeldata() const
	{
		return (ion_uint8*)(m_LockedRect.pBits);
	}

	ion_uint32 D3D9Cubemap::stride() const
	{
		return m_LockedRect.Pitch;
	}

	ion_uint32 D3D9Cubemap::bytesPerPixel() const
	{
		return pixelformatsize(m_Pixelformat);
	}

	ion_uint32 D3D9Cubemap::width() const
	{
		return m_LevelWidth;
	}

	ion_uint32 D3D9Cubemap::height() const
	{
		return m_LevelHeight;
	}

	LPDIRECT3DCUBETEXTURE9 D3D9Cubemap::d3d9cubetexture() const
	{
		return m_pCubemap;
	}

	void D3D9Cubemap::d3d9releaseBeforeReset()
	{
		m_IsDataOK=false;
	}

	void D3D9Cubemap::d3d9restoreAfterReset()
	{
		m_IsDataOK=false;
	}

	/*
	video::Pixelformat D3D9Cubemap::actualpixelformat() const
	{
		return m_ActualPixelformat;
	}

	bool D3D9Cubemap::isValid() const
	{
		return m_IsValid;
	}

	bool D3D9Cubemap::isDataOK() const
	{
		return m_IsDataOK;
	}

	void D3D9Cubemap::dataIsOK() { m_IsDataOK=true; }

	LPDIRECT3DCUBETEXTURE9 D3D9Cubemap::d3d9cubetexture() const
	{
		return m_pCubemap;
	}

	ion_uint32 D3D9Cubemap::edgelength() const
	{
		return m_Edgelength;
	}

	ion_uint32 D3D9Cubemap::numMipmaplevels() const
	{
		return m_NumMipmaplevels;
	}

	void D3D9Cubemap::transferData(const ion_uint32 level,const base::Datablock& datablock,const ion_uint32 pitch,
		const video::Pixelformat& pixelformat)
	{
		D3DCUBEMAP_FACES face;
		switch (m_CurrentFace) {
			case FrontFace:face=D3DCUBEMAP_FACE_POSITIVE_Z; break;
			case BackFace:face=D3DCUBEMAP_FACE_POSITIVE_Z; break;
			case LeftFace:face=D3DCUBEMAP_FACE_POSITIVE_X; break;
			case RightFace:face=D3DCUBEMAP_FACE_POSITIVE_X; break;
			case TopFace:face=D3DCUBEMAP_FACE_POSITIVE_Y; break;
			case BottomFace:face=D3DCUBEMAP_FACE_POSITIVE_Y; break;
			default:face=D3DCUBEMAP_FACE_POSITIVE_X;
		}
		D3DLOCKED_RECT lockedrect;
		m_pCubemap->LockRect(face,level,&lockedrect,0,0);
		BYTE *pDest=(BYTE *)(lockedrect.pBits);
		const BYTE *pSrc=datablock.data();
		UINT y=0;
		UINT bpl=(pitch<(UINT)(lockedrect.Pitch)) ? pitch : (UINT)(lockedrect.Pitch);
		for (;y<m_Edgelength;pSrc+=pitch,pDest+=lockedrect.Pitch,++y) {
			memcpy(pDest,pSrc,bpl);
		}
		m_pCubemap->UnlockRect(face,level);
	}

	void D3D9Cubemap::d3d9releaseBeforeReset()
	{
		m_IsDataOK=false;
	}

	void D3D9Cubemap::d3d9restoreAfterReset()
	{
		m_IsDataOK=false;
	}*/



}
}
