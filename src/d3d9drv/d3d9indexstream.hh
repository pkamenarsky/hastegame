#ifndef ION_D3D9DRV_D3D9INDEXSTREAM_HH_INCLUDED
#define ION_D3D9DRV_D3D9INDEXSTREAM_HH_INCLUDED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef DIRECT3D_VERSION
#define DIRECT3D_VERSION 0x0900
#endif

#include <windows.h>
#include <d3d9.h>

#include "../video/indexstream.hh"

namespace ion {
namespace d3d9drv {

	class D3D9Device;

	class D3D9Indexstream:public video::Indexstream
	{
	public:
		D3D9Indexstream(D3D9Device& d3d9device,const video::Indexformat format,const ion_uint32 flags,
			const video::Memorypool mempool,const ion_uint32 _capacity);
		~D3D9Indexstream();

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK();

		void bind();
		void map(const ion_uint32 flags);
		void unmap();

		void* mappedPointer();
		bool isMapped() const;

		void d3d9releaseBeforeReset();
		void d3d9restoreAfterReset();

		ion_uint32 index(const ion_uint32 position);
		void index(const ion_uint32 position,const ion_uint32 newindex);

	protected:

		void* m_pMappedPointer;
		D3D9Device& m_rD3D9Device;
		LPDIRECT3DINDEXBUFFER9 m_pIB;
		bool m_IsDataOK;
	};

}
}

#endif
