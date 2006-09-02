#include "renderjob.hh"
#include "shader.hh"

namespace ion {
namespace scene {

	Renderjob::Renderjob(const Renderable& rRenderable)
	{
		clear();
		m_pRenderable=&rRenderable;
	}

	Renderjob::Renderjob(const Renderjob& src)
	{
		clear();
		this->clone(src);
	}

	void Renderjob::clear()
	{
		//Propertytable::clear(); // TODO: clear() has to be optimized!

		m_Worldtransform.position(0,0,0);
		m_Worldtransform.rotation().x()=0;
		m_Worldtransform.rotation().y()=0;
		m_Worldtransform.rotation().z()=0;
		m_Worldtransform.rotation().w()=1;

		m_FirstElement=0;
		m_NumElements=0;
		m_IndexOffset=0;
		m_pVertexstream=0;
		m_pIndexstream=0;
		m_pRenderable=0;
		m_pShader=0;
		m_pCamera=0;
		m_pPropertytable=0;
		m_ViewerDistance=0;
	}

	void Renderjob::clone(const Renderjob& src)
	{
		//Propertytable::clone(src);
		m_Worldtransform=src.worldtransform();
		m_PrimitivesType=src.primitivestype();
		m_FirstElement=src.firstElement();
		m_NumElements=src.numElements();
		m_IndexOffset=src.indexOffset();
		m_pVertexstream=&(src.vertexstream());
		m_pIndexstream=src.indexstream();
		m_pRenderable=&(src.renderable());
		m_pShader=src.shader();
		m_pCamera=src.rendertargetCamera();
		m_pPropertytable=src.propertytable();
		m_ViewerDistance=src.viewerDistance();
	}

	Renderjob& Renderjob::operator =(const Renderjob& src)
	{
		clone(src);
		return *this;
	}

	const math::Transform& Renderjob::worldtransform() const
	{
		return m_Worldtransform;
	}

	math::Transform& Renderjob::worldtransform()
	{
		return m_Worldtransform;
	}

	void Renderjob::primitivestype(const video::PrimitivesType type)
	{
		m_PrimitivesType=type;
	}

	video::PrimitivesType Renderjob::primitivestype() const
	{
		return m_PrimitivesType;
	}

	void Renderjob::firstElement(const unsigned long first)
	{
		m_FirstElement=first;
	}

	unsigned long Renderjob::firstElement() const
	{
		return m_FirstElement;
	}

	void Renderjob::numElements(const unsigned long num)
	{
		m_NumElements=num;
	}

	unsigned long Renderjob::numElements() const
	{
		return m_NumElements;
	}

	void Renderjob::indexOffset(const unsigned long offset)
	{
		m_IndexOffset=offset;
	}

	unsigned long Renderjob::indexOffset() const
	{
		return m_IndexOffset;
	}

	void Renderjob::vertexstream(video::Vertexstream& rVertexstream)
	{
		m_pVertexstream=&rVertexstream;
	}

	video::Vertexstream& Renderjob::vertexstream() const
	{
		return *m_pVertexstream;
	}

	void Renderjob::indexstream(video::Indexstream* pIndexstream)
	{
		m_pIndexstream=pIndexstream;
	}

	video::Indexstream* Renderjob::indexstream() const
	{
		return m_pIndexstream;
	}

	void Renderjob::shader(Shader* pShader)
	{
		m_pShader=(pShader==0) ? 0 : (pShader->isValid() ? pShader : 0);
	}

	Shader* Renderjob::shader() const
	{
		return m_pShader;
	}

	void Renderjob::rendertargetCamera(Camera *pCamera)
	{
		m_pCamera=pCamera;
	}

	Camera* Renderjob::rendertargetCamera() const
	{
		return m_pCamera;
	}

	bool Renderjob::isValid() const
	{
		return (m_pVertexstream!=0);
	}

	void Renderjob::renderable(const Renderable& rRenderable)
	{
		m_pRenderable=&rRenderable;
	}

	const Renderable& Renderjob::renderable() const
	{
		return *m_pRenderable;
	}

	void Renderjob::propertytable(video::Propertytable *pPropertytable)
	{
		m_pPropertytable=pPropertytable;
	}

	video::Propertytable* Renderjob::propertytable() const
	{
		return m_pPropertytable;
	}

	void Renderjob::viewerDistance(const float num)
	{
		m_ViewerDistance=num;
	}

	float Renderjob::viewerDistance() const
	{
		return m_ViewerDistance;
	}

	void Renderjob::drawIndexed(video::Videodevice &rVideodevice)
	{
		rVideodevice.drawIndexedPrimitives(
			m_PrimitivesType,*m_pVertexstream,*m_pIndexstream,
			m_IndexOffset,m_NumElements);
	}

	void Renderjob::draw(video::Videodevice &rVideodevice)
	{
		rVideodevice.drawPrimitives(m_PrimitivesType,*m_pVertexstream,m_FirstElement,m_NumElements);
	}

}
}
