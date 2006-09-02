#ifndef ION_VIDEO_MEMINDEXSTREAM_HH_INCLUDED
#define ION_VIDEO_MEMINDEXSTREAM_HH_INCLUDED

#include "../base/datablock.hh"
#include "../base/dll.hh"
#include "../base/iontypes.hh"
#include "indexstream.hh"

namespace ion {
namespace video {

	class ION_API MemIndexstream:public Indexstream
	{
	public:
		MemIndexstream(const ion_uint32 size);

		inline void bind() {}
		inline void map(const ion_uint32 flags) {}
		inline void unmap() {}

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK() {}

		inline void* mappedPointer() { return m_Indices.data(); }
		inline const void* mappedPointer() const { return m_Indices.data(); }
		inline bool isMapped() const { return true; }

		inline ion_uint32 index(const ion_uint32 position) { return ((ion_uint32*)m_Indices.data())[position]; }
		inline void index(const ion_uint32 position,const ion_uint32 newindex) { ((ion_uint32*)m_Indices.data())[position]=newindex; }

	protected:
		base::Datablock m_Indices;
	};

}
}

#endif
