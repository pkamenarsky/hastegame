#include <algorithm>
#include <vector>

#include "light.hh"
#include "lightlist.hh"

namespace ion {
namespace scene {

	struct Lightlist::Lightarray
	{
		std::vector< Light* > m_Lights,m_Pointlights,m_DirectionalLights,m_Spotlights;

		std::vector< Light* >::iterator find(Light* pLight)
		{
			return std::find(m_Lights.begin(),m_Lights.end(),pLight);
		}

		bool contains(Light* pLight)
		{
			return (find(pLight)!=m_Lights.end());
		}
	};
	
	
	

	Lightlist::Lightlist():m_ReferenceCounting(false),m_pLightarray(new Lightarray)
	{
		m_pLightarray->m_Lights.reserve(64);
	}

	Lightlist::~Lightlist()
	{
		delete m_pLightarray;
	}

	void Lightlist::addLight(Light &rLight)
	{
		//if (m_pLightarray->contains(&rLight)) return;

		if (m_ReferenceCounting) rLight.addRef();
		m_pLightarray->m_Lights.push_back(&rLight);

		switch (rLight.lighttype()) {
			case video::Lighttype_Point:m_pLightarray->m_Pointlights.push_back(&rLight); break;
			case video::Lighttype_Directional:m_pLightarray->m_DirectionalLights.push_back(&rLight); break;
			case video::Lighttype_Spot:m_pLightarray->m_Spotlights.push_back(&rLight); break;
			default:break;
		}
	}

	void Lightlist::removeLight(Light &rLight)
	{
		std::vector< Light * >::iterator it=
			std::find(m_pLightarray->m_Lights.begin(),m_pLightarray->m_Lights.end(),&rLight);
		if (it==m_pLightarray->m_Lights.end()) return;

		m_pLightarray->m_Lights.erase(it);

		switch (rLight.lighttype()) {
			case video::Lighttype_Point:
				it=std::find(m_pLightarray->m_Pointlights.begin(),m_pLightarray->m_Pointlights.end(),&rLight);
				m_pLightarray->m_Pointlights.erase(it);
				break;
			case video::Lighttype_Directional:
				it=std::find(m_pLightarray->m_DirectionalLights.begin(),m_pLightarray->m_DirectionalLights.end(),&rLight);
				m_pLightarray->m_DirectionalLights.erase(it);
				break;
			case video::Lighttype_Spot:
				it=std::find(m_pLightarray->m_Spotlights.begin(),m_pLightarray->m_Spotlights.end(),&rLight);
				m_pLightarray->m_Spotlights.erase(it);
				break;
			default:break;
		}

		if (m_ReferenceCounting) rLight.release();

	}

	ion_uint32 Lightlist::numLights() const
	{
		return (ion_uint32)(m_pLightarray->m_Lights.size());
	}

	ion_uint32 Lightlist::numPointlights() const
	{
		return (ion_uint32)(m_pLightarray->m_Pointlights.size());
	}

	ion_uint32 Lightlist::numDirectionalLights() const
	{
		return (ion_uint32)(m_pLightarray->m_DirectionalLights.size());
	}

	ion_uint32 Lightlist::numSpotlights() const
	{
		return (ion_uint32)(m_pLightarray->m_Spotlights.size());
	}

	void Lightlist::clear()
	{
		//m_pLightarray->m_Lights.clear();
		m_pLightarray->m_Lights.resize(0);
		m_pLightarray->m_Pointlights.resize(0);
		m_pLightarray->m_DirectionalLights.resize(0);
		m_pLightarray->m_Spotlights.resize(0);
	}

	Light* Lightlist::light(const ion_uint32 index)
	{
		return (index<numLights()) ? m_pLightarray->m_Lights[index] : 0;
	}

	const Light* Lightlist::light(const ion_uint32 index) const
	{
		return (index<numLights()) ? m_pLightarray->m_Lights[index] : 0;
	}

	Light* Lightlist::pointlight(const ion_uint32 index)
	{
		return (index<numPointlights()) ? m_pLightarray->m_Pointlights[index] : 0;
	}

	const Light* Lightlist::pointlight(const ion_uint32 index) const
	{
		return (index<numPointlights()) ? m_pLightarray->m_Pointlights[index] : 0;
	}

	Light* Lightlist::directionalLight(const ion_uint32 index)
	{
		return (index<numDirectionalLights()) ? m_pLightarray->m_DirectionalLights[index] : 0;
	}

	const Light* Lightlist::directionalLight(const ion_uint32 index) const
	{
		return (index<numDirectionalLights()) ? m_pLightarray->m_DirectionalLights[index] : 0;
	}

	Light* Lightlist::spotlight(const ion_uint32 index)
	{
		return (index<numSpotlights()) ? m_pLightarray->m_Spotlights[index] : 0;
	}

	const Light* Lightlist::spotlight(const ion_uint32 index) const
	{
		return (index<numSpotlights()) ? m_pLightarray->m_Spotlights[index] : 0;
	}


	/*
	
	const std::vector< Light * >& Lightlist::lights() const
	{
		return m_pLightarray->m_Lights;
	}

	const std::vector< Light * >& Lightlist::pointlights() const
	{
		return m_pLightarray->m_Pointlights;
	}

	const std::vector< Light * >& Lightlist::directionalLights() const
	{
		return m_pLightarray->m_DirectionalLights;
	}

	const std::vector< Light * >& Lightlist::spotlights() const
	{
		return m_pLightarray->m_Spotlights;
	}*/

	bool Lightlist::isValid() const
	{
		return (m_pLightarray!=0);
	}

}
}
