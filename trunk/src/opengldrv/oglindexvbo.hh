#ifndef ION_OPENGLDRV_OGLINDEXVBO_HH_INCLUDED
#define ION_OPENGLDRV_OGLINDEXVBO_HH_INCLUDED

#include "../base/dll.hh"
#include "../video/indexiterator.hh"
#include "../video/indexstream.hh"
#include "../video/memindexstream.hh"

namespace ion {
namespace opengldrv {

	class BufferObject;
	class OGLDevice;

	class ION_API OGLIndexVBO:public video::Indexstream
	{
	public:
		OGLIndexVBO(OGLDevice& rOGLDevice,
			const ion_uint32 numIndices,const video::Indexformat format,const ion_uint32 flags);
		~OGLIndexVBO();

		bool isValid() const;
		bool isDataOK() const;
		void dataIsOK();

		void bind();
		void map(const ion_uint32 flags);
		void unmap();

		void* mappedPointer();
		bool isMapped() const;

		ion_uint32 index(const ion_uint32 position);
		void index(const ion_uint32 position,const ion_uint32 newindex);
	protected:

		BufferObject *m_pBuffer;
		OGLDevice& m_rOGLDevice;
		bool m_IsDataOK;
	};

}
}


#endif
