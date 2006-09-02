#include "ogldevice.hh"
#include "oglindexstream.hh"

namespace ion {
namespace opengldrv {

	// BUG: 16 bit indices don't work on some systems - either a bug in the code or an OpenGL issue#
	// as a workaround, 32 bits are used always, no matter what is specified as format

	OGLIndexstream::OGLIndexstream(OGLDevice& rOGLDevice,const ion_uint32 numIndices,
	const video::Indexformat format,const ion_uint32 flags):video::Indexstream(video::Indexformat_32bit,numIndices),
	m_pMemIndexstream(0),m_pCurrentBackend(0),m_pOGLIndexVBO(0),m_pOGLIndexVA(0),m_pOGLIndexImmediate(0),
	m_Flags(flags),m_rOGLDevice(rOGLDevice)
	{
		switchStorage(rOGLDevice.vertexStorageMode());
		if (!(flags&video::Streamflags_Dynamic))
			m_pMemIndexstream=new video::MemIndexstream(numIndices);
	}

	OGLIndexstream::~OGLIndexstream()
	{
		if (m_pOGLIndexVBO) delete m_pOGLIndexVBO;
		if (m_pOGLIndexVA) delete m_pOGLIndexVA;
		if (m_pOGLIndexImmediate) delete m_pOGLIndexImmediate;
		if (m_pMemIndexstream) delete m_pMemIndexstream;
	}

	bool OGLIndexstream::isValid() const
	{
		return (m_pCurrentBackend!=0) ? m_pCurrentBackend->isValid() : false;
	}

	bool OGLIndexstream::isDataOK() const
	{
		return m_pCurrentBackend->isDataOK();
	}

	void OGLIndexstream::dataIsOK() { m_pCurrentBackend->dataIsOK(); }

	void *OGLIndexstream::mappedPointer()
	{
		return m_pCurrentBackend->mappedPointer();
	}

	bool OGLIndexstream::isMapped() const
	{
		return m_pCurrentBackend->isMapped();
	}

	void OGLIndexstream::bind()
	{
		m_pCurrentBackend->bind();
	}

	void OGLIndexstream::map(const ion_uint32 flags)
	{
		m_pCurrentBackend->map(flags);
	}

	void OGLIndexstream::unmap()
	{
		m_pCurrentBackend->unmap();
	}

	void OGLIndexstream::restoreData()
	{
		if (m_pMemIndexstream) {
			m_pCurrentBackend->map(video::Map_Writeonly);
			m_pCurrentBackend->transferFrom(*m_pMemIndexstream);
			m_pCurrentBackend->unmap();
		}
	}

	void OGLIndexstream::switchStorage(const int type)
	{
		video::Indexstream *pNewBackend=0;

		switch (type) {
			case OGLDevice::OGLVertexStorage_VBO:
				if (m_pOGLIndexVBO!=0) return;
				m_pOGLIndexVBO=new OGLIndexVBO(m_rOGLDevice,m_Capacity,m_Format,m_Flags);
				m_pOGLIndexVA=0;
				m_pOGLIndexImmediate=0;
				pNewBackend=m_pOGLIndexVBO;
				break;
			case OGLDevice::OGLVertexStorage_VA:
				if (m_pOGLIndexVA!=0) return;
				if (m_pOGLIndexVBO) glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
				m_pOGLIndexVA=new OGLIndexVA(m_rOGLDevice,m_Capacity,m_Format,m_Flags);
				m_pOGLIndexVBO=0;
				m_pOGLIndexImmediate=0;
				pNewBackend=m_pOGLIndexVA;
				break;
			case OGLDevice::OGLVertexStorage_Immediate:
				if (m_pOGLIndexVA!=0) return;
				if (m_pOGLIndexVBO) glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
				m_pOGLIndexImmediate=new OGLIndexImmediate(m_rOGLDevice,m_Capacity,m_Format,m_Flags);
				m_pOGLIndexVBO=0;
				m_pOGLIndexVA=0;
				pNewBackend=m_pOGLIndexImmediate;
				break;
			default:return; // NOTE: return, not break - it shall leave when no known backend is specified
		}

		if (m_pCurrentBackend) delete m_pCurrentBackend;
		m_pCurrentBackend=pNewBackend;

		restoreData();
	}

	ion_uint32 OGLIndexstream::index(const ion_uint32 position)
	{
		return (m_pMemIndexstream!=0) ? m_pMemIndexstream->index(position) : m_pCurrentBackend->index(position);
	}

	void OGLIndexstream::index(const ion_uint32 position,const ion_uint32 newindex)
	{
		m_pCurrentBackend->index(position,newindex);
		if (m_pMemIndexstream!=0) m_pMemIndexstream->index(position,newindex);
	}

}
}
