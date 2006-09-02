#include "d3d9device.hh"
#include "d3d9indexstream.hh"
#include "tod3d9convs.hh"
#include "../video/indexformat.hh"

namespace ion {
namespace d3d9drv {

	D3D9Indexstream::D3D9Indexstream(D3D9Device& d3d9device,const video::Indexformat format,const ion_uint32 flags,
		const video::Memorypool mempool,const ion_uint32 _capacity):Indexstream(format,_capacity),
		m_pMappedPointer(0),m_rD3D9Device(d3d9device),m_IsDataOK(true)
	{
		DWORD ibflags=0;

		if (flags&video::Streamflags_Points) ibflags|=D3DUSAGE_POINTS;
		if (flags&video::Streamflags_Writeonly) ibflags|=D3DUSAGE_WRITEONLY;
		if (flags&video::Streamflags_Dynamic) ibflags|=D3DUSAGE_DYNAMIC;

		d3d9device.d3d9deviceinterface()->CreateIndexBuffer(
			video::indexformatSizeLookup(format)*_capacity,ibflags,
			(format==video::Indexformat_32bit) ? D3DFMT_INDEX32 : D3DFMT_INDEX16,
			d3dmempool(mempool),&m_pIB,0);
	}

	D3D9Indexstream::~D3D9Indexstream()
	{
		unmap();
		if (m_pIB) m_pIB->Release();
	}

	bool D3D9Indexstream::isValid() const
	{
		return (m_pIB!=0);
	}

	bool D3D9Indexstream::isDataOK() const
	{
		return m_IsDataOK;
	}

	void D3D9Indexstream::dataIsOK() { m_IsDataOK=true; }

	void D3D9Indexstream::bind()
	{
		if (m_pMappedPointer!=0) return;

		if (m_rD3D9Device.indexstream()!=this) {
			m_rD3D9Device.indexstream(this);
			m_rD3D9Device.d3d9deviceinterface()->SetIndices(m_pIB);
		}
	}

	void D3D9Indexstream::map(const ion_uint32 flags)
	{
		if (m_pMappedPointer!=0) return;

		DWORD d3dflags=0;
		if (flags&video::Map_Readonly) d3dflags|=D3DLOCK_READONLY;
		if (flags&video::Map_Discard) d3dflags|=D3DLOCK_DISCARD;
		if (flags&video::Map_Nooverwrite) d3dflags|=D3DLOCK_NOOVERWRITE;

		m_pIB->Lock(0,0,&m_pMappedPointer,d3dflags);
	}

	void D3D9Indexstream::unmap()
	{
		if (m_pMappedPointer==0) return;
		m_pIB->Unlock();
		m_pMappedPointer=0;
	}

	void* D3D9Indexstream::mappedPointer()
	{
		return m_pMappedPointer;
	}

	bool D3D9Indexstream::isMapped() const
	{
		return (m_pMappedPointer!=0);
	}

	ion_uint32 D3D9Indexstream::index(const ion_uint32 position)
	{
		if (position<capacity()) {
			switch (m_Format) {
				case video::Indexformat_16bit:return (ion_uint32)( ((ion_uint16*)m_pMappedPointer)[position] );
				case video::Indexformat_32bit:return ((ion_uint32*)m_pMappedPointer)[position];
			}
		}

		return 0xffffffff;
	}

	void D3D9Indexstream::index(const ion_uint32 position,const ion_uint32 newindex)
	{
		if (position<capacity()) {
			switch (m_Format) {
				case video::Indexformat_16bit:((ion_uint16*)m_pMappedPointer)[position]=newindex&0xFFFF; break;
				case video::Indexformat_32bit:((ion_uint32*)m_pMappedPointer)[position]=newindex; break;
			}
		}
	}

	void D3D9Indexstream::d3d9releaseBeforeReset()
	{
		m_IsDataOK=false;
	}

	void D3D9Indexstream::d3d9restoreAfterReset()
	{
		m_IsDataOK=false;
	}


}
}
