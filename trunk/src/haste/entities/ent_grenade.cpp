#include "ent_grenade.h"

EntGrenade::EntGrenade(EngineExport *EE) : Entity(EE)
{
	ClassName = "grenade";

	SetAABB(Vector3f(-5, -5, -4), Vector3f(5, 5, 5));

	exploded = FALSE;
	time = 2;

	bounce[0] = EE->LoadSound(String("basehaste/sound/weapons/grenade/hgrenb1a.wav"), FALSE);
	bounce[1] = EE->LoadSound(String("basehaste/sound/weapons/grenade/hgrenb2a.wav"), FALSE);
	explode = EE->LoadSound(String("basehaste/sound/weapons/rocket/rocklx1a.wav"), FALSE);

	spritecache = EE->NewSpriteCache(500);

	smoke = EE->LoadTexture("basehaste/gfx/smoke01_na.tga", Pixelformat_RGBA8888);

	LoadMesh(String("grenade.obj"));
	EE->AttachToWorld(this);
//	Link();
}

EntGrenade::EntGrenade()
{
	EntGrenade(NULL);
}

EntGrenade::~EntGrenade()
{
	EE.DestroyEntity(light);
	EE.DetachFromWorld(this);
}

void EntGrenade::Init()
{
	light = EE.SpawnEntity("light", 0);
	light->SetProperty(String("shadows"), String("1"));
	light->SetProperty(String("light"), String("600"));
	light->Init();

	Vector3f	f, r, u;
	AngleVectors(angles, f, r, u);

	velocity = f * 1000;

	last_smoke_trail = origin;
}

void EntGrenade::Think(void)
{
	Vector3f	target;

	Entity::Think();

	if(exploded)
	{
		if(light)
		{
			EE.DestroyEntity(light);
			light = NULL;
		}
	
//		return;
	}
	else
	{
		light->SetOrigin(origin + Vector3f(0, 20, 0));

		time -= EE.Frametime();
		velocity[1] -= 800 * EE.Frametime();

		target = origin + velocity * EE.Frametime();
		EE.trace.TraceBox(origin, target, mins, maxs);

		if(EE.trace.GetFraction() != 1.0f)
		{
			Plane		*plane = EE.trace.GetCollisionPlane();
			Vector3f	reflect;

			//FIXME
			if(plane)
			{
				reflect = velocity - (2 * velocity * plane->normal()) * plane->normal();
				reflect *= 0.5f;	//soften the bounce

				velocity = reflect;
			}

			//play sound
			if(velocity.length() > 80)
				PlaySound(bounce[rand() % 2]);
		}

		if(time <= 0 && !exploded)
		{
			list <Entity*>				elist;
			list <Entity*>::iterator	li;

			Entity						*other;

			EE.DetachFromWorld(this, EF_MODEL);

			EE.GetEntityList(elist, String("classname"), String("player"));
			EE.GetEntityList(elist, String("classname"), String("phys_model"));

			for(li = elist.begin(); li != elist.end(); li++)
			{
				stringstream	sstr;
				Vector3f		push;

				other = *li;

				if(other->GetClassName() == "phys_model")
					push = -(origin - other->GetOrigin()) * 70000 / pow((origin - other->GetOrigin()).length() + 5, 2.0f);
				else
					push = -(origin - other->GetOrigin()) * 17000 / pow((origin - other->GetOrigin()).length() + 5, 2.0f);

				sstr << "airpush" << " " << push[0] << " " << push[1] << " " << push[2];

				EE.ThrowEvent_i(other, sstr.str(), 0);
			}

//			EE.DetachFromWorld(this, EF_MODEL);
//			EE.DestroyEntity(this);

			//play sound
			PlaySound(explode);

			exploded = TRUE;

			return;
		}

		SetOrigin(EE.trace.GetCollisionPoint());

		float	vl = velocity.length();
		if(vl > 80)
			SetAngles(GetAngles() + Vector3f(30 * vl / 600, 30 * vl / 600, 30 * vl / 600));
		else
		{
#if 0
			angles[0] = angles[0] - (int)(angles[0] / (3.14159265f * 2)) * 3.14159265f * 2;
			if(angles[0] < 3.14159265f)
			{
				if(angles[0] > 0)
					angles[0] -= 0.3;
			}
			else
			{
				if(angles[0] < 3.14159265f * 2)
					angles[0] += 0.3;
			}
#else
			angles[2] = 0;
#endif
		}

		//smoke trails
		if((last_smoke_trail - origin).length() > 5)
		{
			last_smoke_trail = origin;
			LoadSprite(smoke, origin, Vector2f(64, 64));
		}
	}

	spritecache->age(EE.Frametime() * 100);

	ApplyEntityTransformToModel();
	EE.Link(this);
}

void EntGrenade::SetProperty(String &key, String &value)
{
	Entity::SetProperty(key, value);

	if(key == "origin")
	{
		std::stringstream	sstr(value);
		Vector3f			v;
		
		sstr >> v[0] >> v[1] >> v[2];

		VecSwizzle(v);
//		light->position(v[0], v[1], v[2]);

//		logstream() << "POS: " << v[0] << " " << v[1] << " "  << v[2] << "\n";
	}
	else if(key == "_color")
	{
		std::stringstream	sstr(value);
		Vector3f			v;
		
		sstr >> v[0] >> v[1] >> v[2];

//		light->color(v[0], v[1], v[2], 1);
//		logstream() << "COLOR: " << v[0] << " "  << v[1] << " "  << v[2] << "\n";
	}
}

void EntGrenade::GetProperty(String &key, String &value)
{
	Entity::GetProperty(key, value);
}

EXPORT_ENTITY(EntGrenade)
