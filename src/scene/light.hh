#ifndef ION_SCENE_LIGHT_HH_INCLUDED
#define ION_SCENE_LIGHT_HH_INCLUDED

#include "../base/numericarrays.hh"
#include "../math/vectors.hh"
#include "../video/coreenums.hh"
#include "node.hh"
#include "indexlist.hh"
#include "renderablelist.hh"
//#include "renderjoblist.hh"

namespace ion {

namespace video {
	class Texture2D;
	class Cubemap;
}

namespace scene {

	class ION_API Light:public Node
	{
	public:
		Light(const base::String& identifier);
		~Light();

		const math::SphereVolume& boundingSphere() const;

		const char *type() const;
		void respawn(base::Streamable& source);
		void serialize(base::Streamable& dest);

		bool isValid() const;

		const base::Float4& color() const;
		base::Float4& color();
		void color(const float a,const float r,const float g,const float b);

		const base::Float4& specularColor() const;
		base::Float4& specularColor();
		void specularColor(const float a,const float r,const float g,const float b);

		const math::Vector3f& direction() const;
		math::Vector3f& direction();
		void direction(const float x,const float y,const float z);

		void constantAttenuation(const float attenuation);
		void linearAttenuation(const float attenuation);
		void quadraticAttenuation(const float attenuation);

		float constantAttenuation() const;
		float linearAttenuation() const;
		float quadraticAttenuation() const;

		void range(const float r);
		float range() const;

		void falloff(const float f);
		float falloff() const;

		void specularExponent(const float f);
		float specularExponent() const;

		void innerConeAngle(const float angle);
		float innerConeAngle() const;

		void outerAngle(const float angle);
		float outerAngle() const;

		void castsShadows(const bool state);
		bool castsShadows() const;

		void projectedTexture(video::Texture2D* pProjectedTexture);
		video::Texture2D* projectedTexture() const;

		void cubemap(video::Cubemap* pCubemap);
		video::Cubemap* cubemap() const;

		void lighttype(const video::Lighttype lighttype);
		video::Lighttype lighttype() const;

		Renderablelist& renderablelist();
		const Renderablelist& renderablelist() const;

		Indexlist& renderableIndicesList();
		const Indexlist& renderableIndicesList() const;

		/*Renderjoblist& renderjoblist();
		const Renderjoblist& renderjoblist() const;*/

	protected:
		Renderablelist m_Renderablelist;
		Indexlist m_RenderableIndicesList;
		//Renderjoblist m_Renderjoblist;
		base::Float4 m_Color,m_SpecularColor;
		math::Vector3f m_Direction;
		video::Cubemap *m_pCubemap;
		video::Texture2D *m_pProjectedTexture;
		float m_Range,m_Falloff,m_SpecularExponent,m_InnerConeAngle,m_OuterAngle,m_Attenuation[3];
		bool m_CastsShadows;
		math::SphereVolume m_Sphere;
		video::Lighttype m_Lighttype;
	};

}
}

#endif
