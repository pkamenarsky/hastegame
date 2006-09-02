#include "light.hh"
#include "renderable.hh"
#include "renderquery.hh"
#include "simplespace.hh"

namespace ion {
namespace scene {

	Simplespace::Simplespace()
	{
	}

	Simplespace::~Simplespace()
	{
	}

	void Simplespace::determineVisibity(Renderquery& query)
	{
		if (query.camera()==0) return;

		query.lightlist().clear();
		query.renderablelist().clear();
		query.lightIndicesList().clear();
		query.renderableIndicesList().clear();

		for (ion_uint32 l=0;l<m_Lightlist.numLights();++l) {
			Light* pLight=m_Lightlist.light(l);

			bool add=(pLight->lighttype()==video::Lighttype_Directional);
			if (!add) add=(query.camera()->frustum().checkSphere(pLight->transform().position(),pLight->range())!=math::Frustum::NotInFrustum);

			if (add) {
				pLight->renderablelist().clear();
				pLight->renderableIndicesList().clear();
				query.lightlist().addLight(*pLight);
				query.lightIndicesList().addIndex(l);
			}
		}

		for (ion_uint32 r=0;r<m_Renderablelist.numRenderables();++r) {
			Renderable* pRenderable=m_Renderablelist.renderable(r);
			const math::SphereVolume &sphere=pRenderable->boundingSphere();
			math::Vector3f pos=pRenderable->transform().position()+pRenderable->boundingSphere().center();
			if (query.camera()->frustum().checkSphere(pos,sphere.radius()))
			{
				pRenderable->lightlist().clear();

				Lightlist &llist=query.lightlist();
				for (ion_uint32 l=0;l<llist.numLights();++l) {
					Light* pLight=llist.light(l);
					ion_uint32 lightidx=query.lightIndicesList().index(l);
					float lensq=(pos-pLight->transform().position()).lengthSquared();
					float range=pLight->range()+pRenderable->boundingSphere().radius();
					if ((pLight->lighttype()==video::Lighttype_Directional) || (lensq<(range*range) )) {
						pRenderable->lightlist().addLight(*pLight);
						//pRenderable->lightIndicesList().addIndex(lightidx);
						pLight->renderablelist().addRenderable(*pRenderable);
						pLight->renderableIndicesList().addIndex(r);
					}
				}

				query.renderablelist().addRenderable(*pRenderable);
				query.renderableIndicesList().addIndex(r);
			}
		}
	}

	void Simplespace::findInRadius(Renderquery& query,const math::Vector3f point,const float radius)
	{
	}

	void Simplespace::findNearest(Renderquery& query,const math::Vector3f point,const ion_uint32 maxNodes)
	{
	}

	Renderablelist& Simplespace::renderablelist()
	{
		return m_Renderablelist;
	}

	const Renderablelist& Simplespace::renderablelist() const
	{
		return m_Renderablelist;
	}

	Lightlist& Simplespace::lightlist()
	{
		return m_Lightlist;
	}

	const Lightlist& Simplespace::lightlist() const
	{
		return m_Lightlist;
	}


}
}
