#ifndef ION_OPENGLDRV_OGLINDEXVA_HH_INCLUDED
#define ION_OPENGLDRV_OGLINDEXVA_HH_INCLUDED

#include "../base/dll.hh"
#include "../video/indexiterator.hh"
#include "../video/indexstream.hh"

namespace ion {
namespace opengldrv {

	class OGLDevice;

	class ION_API OGLIndexVA:public video::Indexstream
	{
	public:
		OGLIndexVA(OGLDevice& rOGLDevice,
			const ion_uint32 numIndices,const video::Indexformat format,const ion_uint32 flags);
		~OGLIndexVA();

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

		unsigned int *m_pIndices;
		bool m_IsDataOK;
		OGLDevice& m_rOGLDevice;
	};

}
}


#endif
