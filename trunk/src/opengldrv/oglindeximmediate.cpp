#include "oglindeximmediate.hh"
#include "../video/coreenums.hh"

namespace ion {
namespace opengldrv {

	OGLIndexImmediate::OGLIndexImmediate(OGLDevice& rOGLDevice,const ion_uint32 numIndices,
	const video::Indexformat format,const ion_uint32 flags):
	video::Indexstream(video::Indexformat_32bit,numIndices),m_IsDataOK(true),m_rOGLDevice(rOGLDevice)
	{
		m_pIndices=new unsigned int[numIndices];
	}

	OGLIndexImmediate::~OGLIndexImmediate()
	{
		delete m_pIndices;
	}

	bool OGLIndexImmediate::isValid() const
	{
		return (m_pIndices!=0);
	}

	bool OGLIndexImmediate::isDataOK() const
	{
		return m_IsDataOK;
	}

	void OGLIndexImmediate::dataIsOK() { m_IsDataOK=true; }

	void *OGLIndexImmediate::mappedPointer()
	{
		return m_pIndices;
	}

	bool OGLIndexImmediate::isMapped() const
	{
		return true;
	}
	
	void OGLIndexImmediate::bind()
	{
	}

	void OGLIndexImmediate::map(const ion_uint32 flags)
	{
	}

	void OGLIndexImmediate::unmap()
	{
	}

	ion_uint32 OGLIndexImmediate::index(const ion_uint32 position)
	{
		return (position<capacity()) ? m_pIndices[position] :  0xffffffff;
	}

	void OGLIndexImmediate::index(const ion_uint32 position,const ion_uint32 newindex)
	{
		if (position<capacity())
			m_pIndices[position]=newindex;
	}

}
}
