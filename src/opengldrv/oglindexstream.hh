#ifndef ION_OPENGLDRV_OGLINDEXSTREAM_HH_INCLUDED
#define ION_OPENGLDRV_OGLINDEXSTREAM_HH_INCLUDED

#include "../base/dll.hh"
#include "../video/indexiterator.hh"
#include "../video/indexstream.hh"
#include "../video/memindexstream.hh"
#include "oglindeximmediate.hh"
#include "oglindexva.hh"
#include "oglindexvbo.hh"

namespace ion {
namespace opengldrv {

	class OGLDevice;

	class ION_API OGLIndexstream:public video::Indexstream
	{
	public:
		OGLIndexstream(OGLDevice& rOGLDevice,
			const ion_uint32 numIndices,const video::Indexformat format,const ion_uint32 flags);
		~OGLIndexstream();

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK();

		void bind();
		void map(const ion_uint32 flags);
		void unmap();

		void* mappedPointer();
		bool isMapped() const;

		void restoreData();

		void switchStorage(const int type);

		ion_uint32 index(const ion_uint32 position);
		void index(const ion_uint32 position,const ion_uint32 newindex);
	protected:

		video::MemIndexstream *m_pMemIndexstream;
		video::Indexstream *m_pCurrentBackend;
		OGLIndexVBO *m_pOGLIndexVBO;
		OGLIndexVA *m_pOGLIndexVA;
		OGLIndexImmediate *m_pOGLIndexImmediate;
		ion_uint32 m_Flags;
		OGLDevice& m_rOGLDevice;
	};

}
}


#endif
