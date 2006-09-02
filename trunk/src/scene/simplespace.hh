#ifndef ION_SCENE_SIMPLESPACE_HH_INCLUDED
#define ION_SCENE_SIMPLESPACE_HH_INCLUDED

#include "lightlist.hh"
#include "renderablelist.hh"
#include "space.hh"

namespace ion {
namespace scene {

	class Simplespace:public Space
	{
	public:
		ION_API Simplespace();
		ION_API ~Simplespace();

		ION_API void determineVisibity(Renderquery& query);
		ION_API void findInRadius(Renderquery& query,const math::Vector3f point,const float radius);
		ION_API void findNearest(Renderquery& query,const math::Vector3f point,const ion_uint32 maxNodes);

		ION_API Renderablelist& renderablelist();
		ION_API const Renderablelist& renderablelist() const;

		ION_API Lightlist& lightlist();
		ION_API const Lightlist& lightlist() const;

	protected:
		Lightlist m_Lightlist;
		Renderablelist m_Renderablelist;
	};

}
}

#endif
