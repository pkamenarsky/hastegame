#include "light.hh"

namespace ion {
namespace scene {

	Light::Light(const base::String& identifier):Node(identifier),m_pCubemap(0),m_pProjectedTexture(0),m_Range(0),
		m_Falloff(1),m_SpecularExponent(0),m_InnerConeAngle(3),m_OuterAngle(3),m_CastsShadows(false),
		m_Lighttype(video::Lighttype_Point)
	{
		m_Attenuation[0]=1;
		m_Attenuation[1]=0;
		m_Attenuation[2]=0;
	}

	Light::~Light()
	{
	}

	const math::SphereVolume& Light::boundingSphere() const
	{
		return m_Sphere;
	}

	const char* Light::type() const
	{
		return "Light";
	}

	void Light::respawn(base::Streamable& source)
	{
	}

	void Light::serialize(base::Streamable& dest)
	{
	}

	bool Light::isValid() const
	{
		return true;
	}

	const base::Float4& Light::color() const
	{
		return m_Color;
	}

	base::Float4& Light::color()
	{
		return m_Color;
	}

	void Light::color(const float a,const float r,const float g,const float b)
	{
		m_Color[0]=a;
		m_Color[1]=r;
		m_Color[2]=g;
		m_Color[3]=b;
	}

	const base::Float4& Light::specularColor() const
	{
		return m_SpecularColor;
	}

	base::Float4& Light::specularColor()
	{
		return m_SpecularColor;
	}

	void Light::specularColor(const float a,const float r,const float g,const float b)
	{
		m_SpecularColor[0]=a;
		m_SpecularColor[1]=r;
		m_SpecularColor[2]=g;
		m_SpecularColor[3]=b;
	}


	const math::Vector3f& Light::direction() const
	{
		return m_Direction;
	}

	math::Vector3f& Light::direction()
	{
		return m_Direction;
	}

	void Light::direction(const float x,const float y,const float z)
	{
		m_Direction.x()=x;
		m_Direction.y()=y;
		m_Direction.z()=z;
	}

	void Light::constantAttenuation(const float attenuation)
	{
		m_Attenuation[0]=attenuation;
	}

	void Light::linearAttenuation(const float attenuation)
	{
		m_Attenuation[1]=attenuation;
	}

	void Light::quadraticAttenuation(const float attenuation)
	{
		m_Attenuation[2]=attenuation;
	}

	float Light::constantAttenuation() const
	{
		return m_Attenuation[0];
	}

	float Light::linearAttenuation() const
	{
		return m_Attenuation[1];
	}

	float Light::quadraticAttenuation() const
	{
		return m_Attenuation[2];
	}

	void Light::range(const float r)
	{
		m_Range=r;
		m_Sphere.radius(r);
	}

	float Light::range() const
	{
		return m_Range;
	}

	void Light::falloff(const float f)
	{
		m_Falloff=f;
	}

	float Light::falloff() const
	{
		return m_Falloff;
	}

	void Light::specularExponent(const float f)
	{
		m_SpecularExponent=f;
	}

	float Light::specularExponent() const
	{
		return m_SpecularExponent;
	}

	void Light::innerConeAngle(const float angle)
	{
		m_InnerConeAngle=angle;
	}

	float Light::innerConeAngle() const
	{
		return m_InnerConeAngle;
	}

	void Light::outerAngle(const float angle)
	{
		m_OuterAngle=angle;
	}

	float Light::outerAngle() const
	{
		return m_OuterAngle;
	}

	void Light::castsShadows(const bool state)
	{
		m_CastsShadows=state;
	}

	bool Light::castsShadows() const
	{
		return m_CastsShadows;
	}

	void Light::projectedTexture(video::Texture2D* pProjectedTexture)
	{
		m_pProjectedTexture=pProjectedTexture;
	}

	video::Texture2D* Light::projectedTexture() const
	{
		return m_pProjectedTexture;
	}

	void Light::cubemap(video::Cubemap* pCubemap)
	{
		m_pCubemap=pCubemap;
	}

	video::Cubemap* Light::cubemap() const
	{
		return m_pCubemap;
	}

	void Light::lighttype(const video::Lighttype lighttype)
	{
		m_Lighttype=lighttype;
	}

	video::Lighttype Light::lighttype() const
	{
		return m_Lighttype;
	}

	Indexlist& Light::renderableIndicesList()
	{
		return m_RenderableIndicesList;
	}

	const Indexlist& Light::renderableIndicesList() const
	{
		return m_RenderableIndicesList;
	}


	Renderablelist& Light::renderablelist()
	{
		return m_Renderablelist;
	}

	const Renderablelist& Light::renderablelist() const
	{
		return m_Renderablelist;
	}

	/*Renderjoblist& Light::renderjoblist()
	{
		return m_Renderjoblist;
	}

	const Renderjoblist& Light::renderjoblist() const
	{
		return m_Renderjoblist;
	}*/

}
}
