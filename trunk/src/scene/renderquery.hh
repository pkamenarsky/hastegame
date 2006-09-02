#ifndef ION_SCENE_RENDERQUERY_HH_INCLUDED
#define ION_SCENE_RENDERQUERY_HH_INCLUDED

#include "../base/basememobj.hh"
#include "camera.hh"
#include "indexlist.hh"
#include "lightlist.hh"
#include "renderablelist.hh"

namespace ion {
namespace scene {

	class Renderable;

	class Renderquery:public base::BaseMemObj
	{
	public:
		ION_API Renderquery();
		ION_API Renderquery(const Camera &rCamera);
		ION_API ~Renderquery();

		ION_API const Camera* camera() const;
		ION_API void camera(const Camera* pCamera);

		ION_API Renderablelist& renderablelist();
		ION_API const Renderablelist& renderablelist() const;

		ION_API Lightlist& lightlist();
		ION_API const Lightlist& lightlist() const;

		ION_API Indexlist& renderableIndicesList();
		ION_API const Indexlist& renderableIndicesList() const;

		ION_API Indexlist& lightIndicesList();
		ION_API const Indexlist& lightIndicesList() const;

		ION_API bool isValid() const;

	protected:
		Renderablelist m_Renderablelist;
		Lightlist m_Lightlist;
		Indexlist m_RenderableIndicesList,m_LightIndicesList;
		const Camera *m_pCamera;
	};

}
}

#endif
