#include "renderquery.hh"

namespace ion {
namespace scene {

	Renderquery::Renderquery():m_pCamera(0)
	{
	}

	Renderquery::Renderquery(const Camera &rCamera):m_pCamera(&rCamera)
	{
	}

	Renderquery::~Renderquery()
	{
	}

	const Camera* Renderquery::camera() const
	{
		return m_pCamera;
	}

	void Renderquery::camera(const Camera* pCamera)
	{
		m_pCamera=pCamera;
	}

	Renderablelist& Renderquery::renderablelist()
	{
		return m_Renderablelist;
	}

	const Renderablelist& Renderquery::renderablelist() const
	{
		return m_Renderablelist;
	}

	Lightlist& Renderquery::lightlist()
	{
		return m_Lightlist;
	}

	const Lightlist& Renderquery::lightlist() const
	{
		return m_Lightlist;
	}

	Indexlist& Renderquery::renderableIndicesList()
	{
		return m_RenderableIndicesList;
	}

	const Indexlist& Renderquery::renderableIndicesList() const
	{
		return m_RenderableIndicesList;
	}

	Indexlist& Renderquery::lightIndicesList()
	{
		return m_LightIndicesList;
	}

	const Indexlist& Renderquery::lightIndicesList() const
	{
		return m_LightIndicesList;
	}

	bool Renderquery::isValid() const
	{
		return m_Renderablelist.isValid();
	}

}
}
