#ifndef ION_SCENE_LIGHTLIST_HH_INCLUDED
#define ION_SCENE_LIGHTLIST_HH_INCLUDED

//#include <vector>
#include "../base/basememobj.hh"
#include "../base/iontypes.hh"
#include "../base/string.hh"

namespace ion {
namespace scene {

	class Light;

	class Lightlist:public base::BaseMemObj
	{
	public:
		ION_API Lightlist();
		ION_API ~Lightlist();

		ION_API void addLight(Light &rLight);
		ION_API void removeLight(Light &rLight);
		ION_API ion_uint32 numLights() const;
		ION_API ion_uint32 numPointlights() const;
		ION_API ion_uint32 numDirectionalLights() const;
		ION_API ion_uint32 numSpotlights() const;
		ION_API void clear();
		ION_API Light* light(const ion_uint32 index);
		ION_API const Light* light(const ion_uint32 index) const;
		ION_API Light* pointlight(const ion_uint32 index);
		ION_API const Light* pointlight(const ion_uint32 index) const;
		ION_API Light* directionalLight(const ion_uint32 index);
		ION_API const Light* directionalLight(const ion_uint32 index) const;
		ION_API Light* spotlight(const ion_uint32 index);
		ION_API const Light* spotlight(const ion_uint32 index) const;

		/*const std::vector< Light * >& lights() const;
		const std::vector< Light * >& pointlights() const;
		const std::vector< Light * >& directionalLights() const;
		const std::vector< Light * >& spotlights() const;*/
		//std::vector< Light * >& lights();

		bool isValid() const;

	protected:
		bool m_ReferenceCounting;

	private:
		struct Lightarray;
		Lightarray *m_pLightarray;
	};

}
}


#endif
