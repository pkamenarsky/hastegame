#include <algorithm>
#include <vector>

#include "renderable.hh"
#include "renderablelist.hh"

namespace ion {
namespace scene {

	struct Renderablelist::Renderablearray
	{
		std::vector< Renderable* > m_Renderables;

		std::vector< Renderable* >::iterator find(Renderable* pRenderable)
		{
			return std::find(m_Renderables.begin(),m_Renderables.end(),pRenderable);
		}

		bool contains(Renderable* pRenderable)
		{
			return (find(pRenderable)!=m_Renderables.end());
		}
	};
	
	
	

	Renderablelist::Renderablelist():m_ReferenceCounting(false),m_pRenderablearray(new Renderablearray)
	{
		m_pRenderablearray->m_Renderables.reserve(64);
	}

	Renderablelist::~Renderablelist()
	{
		delete m_pRenderablearray;
	}

	void Renderablelist::addRenderable(Renderable &rRenderable)
	{
		// TODO: in debug build, contains() and push_back() are IMMENSE performance eaters! Fix this!
		//if (m_pRenderablearray->contains(&rRenderable)) return;

		if (m_ReferenceCounting) rRenderable.addRef();
		m_pRenderablearray->m_Renderables.push_back(&rRenderable);
	}

	void Renderablelist::removeRenderable(Renderable &rRenderable)
	{
		std::vector< Renderable* >::iterator it=m_pRenderablearray->find(&rRenderable);
		if (it!=m_pRenderablearray->m_Renderables.end()) {
			m_pRenderablearray->m_Renderables.erase(it);
			if (m_ReferenceCounting) rRenderable.release();
		}
	}

	ion_uint32 Renderablelist::numRenderables() const
	{
		return (ion_uint32)(m_pRenderablearray->m_Renderables.size());
	}

	void Renderablelist::clear()
	{
		//m_pRenderablearray->m_Renderables.clear();
		m_pRenderablearray->m_Renderables.resize(0);
	}

	Renderable* Renderablelist::renderable(const ion_uint32 index)
	{
		return (index<numRenderables()) ? m_pRenderablearray->m_Renderables[index] : 0;
	}

	const Renderable* Renderablelist::renderable(const ion_uint32 index) const
	{
		return (index<numRenderables()) ? m_pRenderablearray->m_Renderables[index] : 0;
	}

	bool Renderablelist::isValid() const
	{
		return (m_pRenderablearray!=0);
	}

}
}
