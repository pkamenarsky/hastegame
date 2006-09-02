#ifndef ION_SCENE_SPACE_HH_INCLUDED
#define ION_SCENE_SPACE_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/iontypes.hh"
#include "../math/frustum.hh"
#include "../math/vectors.hh"

namespace ion {
namespace scene {

	class Renderquery;

	class ION_API Space
	{
	public:
		virtual ~Space() {}

		virtual void determineVisibity(Renderquery& query)=0;
		virtual void findInRadius(Renderquery& query,const math::Vector3f point,const float radius)=0;
		virtual void findNearest(Renderquery& query,const math::Vector3f point,const ion_uint32 maxNodes)=0;

	protected:
		Space() {}
	};

}
}

#endif
