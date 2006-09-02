#include "oglindexva.hh"
#include "../video/coreenums.hh"

namespace ion {
namespace opengldrv {

	// BUG: 16 bit indices don't work on some systems - either a bug in the code or an OpenGL issue
	// as a workaround, 32 bits are used always, no matter what is specified as format

	OGLIndexVA::OGLIndexVA(OGLDevice& rOGLDevice,const ion_uint32 numIndices,
	const video::Indexformat format,const ion_uint32 flags):
	video::Indexstream(video::Indexformat_32bit,numIndices),m_IsDataOK(true),m_rOGLDevice(rOGLDevice)
	{
		m_pIndices=new unsigned int[numIndices];
	}

	OGLIndexVA::~OGLIndexVA()
	{
		delete m_pIndices;
	}

	bool OGLIndexVA::isValid() const
	{
		return (m_pIndices!=0);
	}

	bool OGLIndexVA::isDataOK() const
	{
		return m_IsDataOK;
	}

	void OGLIndexVA::dataIsOK() { m_IsDataOK=true; }

	void *OGLIndexVA::mappedPointer()
	{
		return m_pIndices;
	}

	bool OGLIndexVA::isMapped() const
	{
		return true;
	}
	
	void OGLIndexVA::bind()
	{
	}

	void OGLIndexVA::map(const ion_uint32 flags)
	{
	}

	void OGLIndexVA::unmap()
	{
	}

	ion_uint32 OGLIndexVA::index(const ion_uint32 position)
	{
		return (position<capacity()) ? m_pIndices[position] :  0xffffffff;
	}

	void OGLIndexVA::index(const ion_uint32 position,const ion_uint32 newindex)
	{
		if (position<capacity())
			m_pIndices[position]=newindex;
	}

}
}
