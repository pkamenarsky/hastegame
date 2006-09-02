#include "ent_light.h"

EntLight::EntLight(EngineExport *EE) : Entity(EE)
{
	ClassName = "light";

	SetAABB(Vector3f(-50, -50, -50), Vector3f(50, 50, 50));

	light = new Light("light");

	light->lighttype(video::Lighttype_Point);
	light->color(1,2,2,2);
	light->specularColor(1,8,8,8);
	light->specularExponent(16);
	light->range(400);

	light->castsShadows(false);

	casts_shadows = false;
	ignorelight = false;

//	Link();
}

EntLight::EntLight()
{
	EntLight(NULL);
}

EntLight::~EntLight()
{
	EE.RemoveLight(light);
	delete light;
}

void EntLight::Init()
{
	bool	r_shadows = FALSE;
	cvars::getCvarBoolValue("rshadows", r_shadows);

	if(ignorelight)
		return;

	if(/*r_shadows && */casts_shadows)
		light->castsShadows(true);

	EE.AddLight(light, false);
}

void EntLight::Think(void)
{
	Entity::Think();

	light->position(origin);
}

void EntLight::SetProperty(String &key, String &value)
{
	Entity::SetProperty(key, value);

	if(key == "origin")
	{
		std::stringstream	sstr(value);
		Vector3f			v;
		
		sstr >> v[0] >> v[1] >> v[2];

		VecSwizzle(v);
		light->position(v[0], v[1], v[2]);

	}
	else if(key == "_color")
	{
		std::stringstream	sstr(value);
		Vector3f			v;
		
		sstr >> v[0] >> v[1] >> v[2];

		light->color(1, v[0], v[1], v[2]);
	}
	else if(key == "specular_color")
	{
		std::stringstream	sstr(value);
		Vector3f			v;
		
		sstr >> v[0] >> v[1] >> v[2];

		light->specularColor(1, v[0], v[1], v[2]);
	}
	else if(key == "intensity" || key == "light")
	{
		std::stringstream	sstr(value);
		float				v;
		
		sstr >> v;

		light->range(v);
	}
	else if(key == "specular_exponent")
	{
		std::stringstream	sstr(value);
		float				v;
		
		sstr >> v;

		light->specularExponent(v);
	}
	else if(key == "shadows")
	{
		if(value == "1")
			casts_shadows = true;
	}
	else if(key == "ignorelight")
	{
		if(value == "1")
			ignorelight = true;
	}
}

void EntLight::GetProperty(String &key, String &value)
{
	Entity::GetProperty(key, value);
}

EXPORT_ENTITY(EntLight)
