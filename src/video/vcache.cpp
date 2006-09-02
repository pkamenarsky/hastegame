#include "vertexformat.hh"
#include "vertexstream.hh"
#include "videodevice.hh"
#include "vcache.hh"

namespace ion {
namespace video {

	VCache::VCache(Videodevice& rVideodevice,const Vertexformat& format,const ion_uint32 numVertices):
	m_NumSentVertices(0),m_pVertexstream(0),m_rVideodevice(rVideodevice)
	{
		m_pVertexstream=m_rVideodevice.createVertexstream(numVertices,format,Streamflags_Dynamic/*|Streamflags_Writeonly*/,Mempool_Default); 
		//m_pVertexiterator=new Vertexiterator(*m_pVertexstream);
	}

	VCache::~VCache()
	{
		//if (m_pVertexiterator) delete m_pVertexiterator;
		if (m_pVertexstream) {
			if (m_pVertexstream->isMapped()) m_pVertexstream->unmap();
			delete m_pVertexstream;
		}
	}

	Videodevice& VCache::videodevice() const
	{
		return m_rVideodevice;
	}

	bool VCache::isValid() const
	{
		return (m_pVertexstream!=0) ? m_pVertexstream->isValid() : false;
	}

	void VCache::begin(const PrimitivesType type)
	{
		if (m_pVertexstream->isMapped()) return;

		m_PrimitivesType=type;
		m_pVertexstream->map(Map_Discard/*|Map_Writeonly*/);
		m_NumSentVertices=0;
		m_Vertexposition=0;
		//m_pVertexiterator->positionInStream(0);
	}

	void VCache::pushVertex()
	{
		if (!m_pVertexstream->isMapped()) return;

		/*if ((m_pVertexiterator->positionInStream()+1)>=m_pVertexstream->capacity()) {
			flush();
			m_pVertexiterator->positionInStream(0);
			m_pVertexstream->map(Map_Discard|Map_Writeonly);
		} else ++(*m_pVertexiterator);*/
		if ((m_Vertexposition+1)>=m_pVertexstream->capacity()) {
			flush();
			m_Vertexposition=0;
			m_pVertexstream->map(Map_Discard/*|Map_Writeonly*/);
		} else {
			++m_Vertexposition;
			++m_NumSentVertices;
		}
	}

	void VCache::position(const float x,const float y,const float z)
	{
		m_pVertexstream->position(m_Vertexposition,x,y,z);
	}

	void VCache::position(const math::Vector3f& v)
	{
		m_pVertexstream->position(m_Vertexposition,v);
	}

	void VCache::normal(const float x,const float y,const float z)
	{
		m_pVertexstream->normal(m_Vertexposition,x,y,z);
	}

	void VCache::normal(const math::Vector3f& newNormal)
	{
		m_pVertexstream->normal(m_Vertexposition,newNormal);
	}

	void VCache::diffuseColor(const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b)
	{
		m_pVertexstream->diffuseColor(m_Vertexposition,a,r,g,b);
	}

	void VCache::diffuseColor(const ion_uint32 color)
	{
		m_pVertexstream->diffuseColor(m_Vertexposition,color);
	}

	void VCache::specularColor(const ion_uint8 a,const ion_uint8 r,const ion_uint8 g,const ion_uint8 b)
	{
		m_pVertexstream->specularColor(m_Vertexposition,a,r,g,b);
	}

	void VCache::specularColor(const ion_uint32 color)
	{
		m_pVertexstream->specularColor(m_Vertexposition,color);
	}

	void VCache::texcoord1D(const ion_uint32 texstage,const float newTexcoord1D)
	{
		m_pVertexstream->texcoord1D(m_Vertexposition,texstage,newTexcoord1D);
	}

	void VCache::texcoord2D(const ion_uint32 texstage,const float u,const float v)
	{
		m_pVertexstream->texcoord2D(m_Vertexposition,texstage,u,v);
	}

	void VCache::texcoord2D(const ion_uint32 texstage,const math::Vector2f& newTexcoord2D)
	{
		m_pVertexstream->texcoord2D(m_Vertexposition,texstage,newTexcoord2D);
	}

	void VCache::texcoord3D(const ion_uint32 texstage,const float u,const float v,const float w)
	{
		m_pVertexstream->texcoord3D(m_Vertexposition,texstage,u,v,w);
	}

	void VCache::texcoord3D(const ion_uint32 texstage,const math::Vector3f& newTexcoord3D)
	{
		m_pVertexstream->texcoord3D(m_Vertexposition,texstage,newTexcoord3D);
	}

	void VCache::end()
	{
		//Videodevice *pVideodevice=&m_rVideodevice;
		flush();
	}

	void VCache::flush()
	{
		if (!m_pVertexstream->isMapped()) return;

		m_pVertexstream->unmap();

		//if (m_NumSentVertices==0) return;
		if (m_NumSentVertices>m_pVertexstream->capacity()) m_NumSentVertices=m_pVertexstream->capacity();

		ion_uint32 numElements=0;

		switch (m_PrimitivesType) {
			case Primitives_Points:numElements=m_NumSentVertices; break;
			case Primitives_Lines:numElements=m_NumSentVertices/2; break;
			case Primitives_Linestrip:numElements=m_NumSentVertices-1; break;
			case Primitives_Trianglestrip:numElements=m_NumSentVertices-2; break;
			case Primitives_Trianglefan:numElements=m_NumSentVertices-2; break;
			case Primitives_Triangles:numElements=m_NumSentVertices/3; break;
		}

		m_rVideodevice.drawPrimitives(m_PrimitivesType,*m_pVertexstream,0,numElements);
	
		m_NumSentVertices=0;
		m_Vertexposition=0;
	}

}
}
