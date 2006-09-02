#include "bufobject.hh"
#include "ogldevice.hh"
#include "oglvertexstream.hh"
#include "../video/vertexformat.hh"

namespace ion {
namespace opengldrv {

	OGLVertexstream::OGLVertexstream(OGLDevice& rOGLDevice,const ion_uint32 numVertices,
	const video::Vertexformat& format,const ion_uint32 flags,video::Vertexstream::Vertexsource *pSource):
	Vertexstream(format,numVertices,pSource),m_pMemVertexstream(0),m_pCurrentBackend(0),m_pOGLVertexVBO(0),
	m_pOGLVertexVA(0),m_pOGLVertexImmediate(0),m_Flags(flags),m_rOGLDevice(rOGLDevice)
	{
		switchStorage(rOGLDevice.vertexStorageMode());
		if (!(flags&video::Streamflags_Dynamic))
			m_pMemVertexstream=new video::MemVertexstream(format,numVertices,pSource);
	}

	OGLVertexstream::~OGLVertexstream()
	{
		if (m_pMemVertexstream) delete m_pMemVertexstream;
		if (m_pOGLVertexVBO) delete m_pOGLVertexVBO;
		if (m_pOGLVertexVA) delete m_pOGLVertexVA;
		if (m_pOGLVertexImmediate) delete m_pOGLVertexImmediate;
	}

	bool OGLVertexstream::isValid() const
	{
		return (m_pCurrentBackend!=0) ? m_pCurrentBackend->isValid() : false;
	}

	bool OGLVertexstream::isDataOK() const
	{
		return m_pCurrentBackend->isDataOK();
	}

	void OGLVertexstream::dataIsOK() { m_pCurrentBackend->dataIsOK(); }

	void *OGLVertexstream::mappedPointer()
	{
		return m_pCurrentBackend->mappedPointer();
	}

	bool OGLVertexstream::isMapped() const
	{
		return m_pCurrentBackend->isMapped();
	}

	void OGLVertexstream::restoreData()
	{
		if (m_pMemVertexstream) {
			m_pCurrentBackend->map(video::Map_Writeonly);
			m_pCurrentBackend->transferFrom(*m_pMemVertexstream);
			m_pCurrentBackend->unmap();
		}
	}

	void OGLVertexstream::switchStorage(const int type)
	{
		video::Vertexstream *pNewBackend=0;

		switch (type) {
			case OGLDevice::OGLVertexStorage_VBO:
				if (m_pOGLVertexVBO!=0) return;
				if (m_pOGLVertexVA) m_pOGLVertexVA->unbind();
				m_pOGLVertexVBO=new OGLVertexVBO(m_rOGLDevice,m_Capacity,m_Format,m_Flags,m_pSource);
				m_pOGLVertexVA=0;
				m_pOGLVertexImmediate=0;
				pNewBackend=m_pOGLVertexVBO;
				break;
			case OGLDevice::OGLVertexStorage_VA:
				if (m_pOGLVertexVA!=0) return;
				if (m_pOGLVertexVBO) m_pOGLVertexVBO->unbind();
				m_pOGLVertexVA=new OGLVertexVA(m_rOGLDevice,m_Capacity,m_Format,m_Flags,m_pSource);
				m_pOGLVertexVBO=0;
				m_pOGLVertexImmediate=0;
				pNewBackend=m_pOGLVertexVA;
				break;
			case OGLDevice::OGLVertexStorage_Immediate:
				if (m_pOGLVertexImmediate!=0) return;
				if (m_pOGLVertexVA) m_pOGLVertexVA->unbind();
				if (m_pOGLVertexVBO) m_pOGLVertexVBO->unbind();
				m_pOGLVertexImmediate=new OGLVertexImmediate(m_rOGLDevice,m_Capacity,m_Format,m_Flags,m_pSource);
				m_pOGLVertexVBO=0;
				m_pOGLVertexVA=0;
				pNewBackend=m_pOGLVertexImmediate;
				break;
			default:return; // NOTE: return, not break - it shall leave when no known backend is specified
		}

		if (m_pCurrentBackend) delete m_pCurrentBackend;
		m_pCurrentBackend=pNewBackend;

		restoreData();
	}

	void OGLVertexstream::bind()
	{
		m_pCurrentBackend->bind();
	}

	void OGLVertexstream::map(const ion_uint32 flags)
	{
		m_pCurrentBackend->map(flags);
	}

	void OGLVertexstream::unmap()
	{
		m_pCurrentBackend->unmap();
	}

	const math::Vector3f& OGLVertexstream::position(const ion_uint32 vtxindex) const
	{
		return (m_pMemVertexstream!=0) ? m_pMemVertexstream->position(vtxindex) : m_pCurrentBackend->position(vtxindex);
	}

	const math::Vector3f& OGLVertexstream::normal(const ion_uint32 vtxindex) const
	{
		return (m_pMemVertexstream!=0) ? m_pMemVertexstream->normal(vtxindex) : m_pCurrentBackend->normal(vtxindex);
	}

	ion_uint32 OGLVertexstream::diffuseColor(const ion_uint32 vtxindex) const
	{
		return (m_pMemVertexstream!=0) ? m_pMemVertexstream->diffuseColor(vtxindex) : m_pCurrentBackend->diffuseColor(vtxindex);
	}

	ion_uint32 OGLVertexstream::specularColor(const ion_uint32 vtxindex) const
	{
		return (m_pMemVertexstream!=0) ? m_pMemVertexstream->specularColor(vtxindex) : m_pCurrentBackend->specularColor(vtxindex);
	}

	float OGLVertexstream::texcoord1D(const ion_uint32 vtxindex,const ion_uint32 texstage) const
	{
		return (m_pMemVertexstream!=0) ? m_pMemVertexstream->texcoord1D(vtxindex,texstage) : m_pCurrentBackend->texcoord1D(vtxindex,texstage);
	}

	const math::Vector2f& OGLVertexstream::texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage) const
	{
		return (m_pMemVertexstream!=0) ? m_pMemVertexstream->texcoord2D(vtxindex,texstage) : m_pCurrentBackend->texcoord2D(vtxindex,texstage);
	}

	const math::Vector3f& OGLVertexstream::texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage) const
	{
		return (m_pMemVertexstream!=0) ? m_pMemVertexstream->texcoord3D(vtxindex,texstage) : m_pCurrentBackend->texcoord3D(vtxindex,texstage);
	}

	void OGLVertexstream::position(const ion_uint32 vtxindex,const float x,const float y,const float z)
	{
		if (m_pMemVertexstream!=0) m_pMemVertexstream->position(vtxindex,x,y,z);
		m_pCurrentBackend->position(vtxindex,x,y,z);
	}

	void OGLVertexstream::position(const ion_uint32 vtxindex,const math::Vector3f& newPosition)
	{
		if (m_pMemVertexstream!=0) m_pMemVertexstream->position(vtxindex,newPosition);
		m_pCurrentBackend->position(vtxindex,newPosition);
	}

	void OGLVertexstream::normal(const ion_uint32 vtxindex,const float x,const float y,const float z)
	{
		if (m_pMemVertexstream!=0) m_pMemVertexstream->normal(vtxindex,x,y,z);
		m_pCurrentBackend->normal(vtxindex,x,y,z);
	}

	void OGLVertexstream::normal(const ion_uint32 vtxindex,const math::Vector3f& newNormal)
	{
		if (m_pMemVertexstream!=0) m_pMemVertexstream->normal(vtxindex,newNormal);
		m_pCurrentBackend->normal(vtxindex,newNormal);
	}

	void OGLVertexstream::diffuseColor(const ion_uint32 vtxindex,const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b)
	{
		if (m_pMemVertexstream!=0) m_pMemVertexstream->diffuseColor(vtxindex,a,r,g,b);
		m_pCurrentBackend->diffuseColor(vtxindex,a,r,g,b);
	}

	void OGLVertexstream::diffuseColor(const ion_uint32 vtxindex,const ion_uint32 color)
	{
		if (m_pMemVertexstream!=0) m_pMemVertexstream->diffuseColor(vtxindex,color);
		m_pCurrentBackend->diffuseColor(vtxindex,color);
	}

	void OGLVertexstream::specularColor(const ion_uint32 vtxindex,const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b)
	{
		if (m_pMemVertexstream!=0) m_pMemVertexstream->specularColor(vtxindex,a,r,g,b);
		m_pCurrentBackend->specularColor(vtxindex,a,r,g,b);
	}

	void OGLVertexstream::specularColor(const ion_uint32 vtxindex,const ion_uint32 color)
	{
		if (m_pMemVertexstream!=0) m_pMemVertexstream->specularColor(vtxindex,color);
		m_pCurrentBackend->specularColor(vtxindex,color);
	}

	void OGLVertexstream::texcoord1D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float newTexcoord1D)
	{
		if (m_pMemVertexstream!=0) m_pMemVertexstream->texcoord1D(vtxindex,texstage,newTexcoord1D);
		m_pCurrentBackend->texcoord1D(vtxindex,texstage,newTexcoord1D);
	}

	void OGLVertexstream::texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float u,const float v)
	{
		if (m_pMemVertexstream!=0) m_pMemVertexstream->texcoord2D(vtxindex,texstage,u,v);
		m_pCurrentBackend->texcoord2D(vtxindex,texstage,u,v);
	}

	void OGLVertexstream::texcoord2D(const ion_uint32 vtxindex,const ion_uint32 texstage,const math::Vector2f& newTexcoord2D)
	{
		if (m_pMemVertexstream!=0) m_pMemVertexstream->texcoord2D(vtxindex,texstage,newTexcoord2D);
		m_pCurrentBackend->texcoord2D(vtxindex,texstage,newTexcoord2D);
	}

	void OGLVertexstream::texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage,const float u,const float v,const float w)
	{
		if (m_pMemVertexstream!=0) m_pMemVertexstream->texcoord3D(vtxindex,texstage,u,v,w);
		m_pCurrentBackend->texcoord3D(vtxindex,texstage,u,v,w);
	}

	void OGLVertexstream::texcoord3D(const ion_uint32 vtxindex,const ion_uint32 texstage,const math::Vector3f& newTexcoord3D)
	{
		if (m_pMemVertexstream!=0) m_pMemVertexstream->texcoord3D(vtxindex,texstage,newTexcoord3D);
		m_pCurrentBackend->texcoord3D(vtxindex,texstage,newTexcoord3D);
	}

}
}
