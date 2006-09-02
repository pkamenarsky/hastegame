#include "ent_rocket.h"

EntRocket::EntRocket(EngineExport *EE) : Entity(EE)
{
	ClassName = "rocket";

	SetAABB(Vector3f(-5, -5, -4), Vector3f(5, 5, 5));

	exploded = FALSE;

	time = 0;

	spritecache = EE->NewSpriteCache(500);
//	spritecache = NULL;

	boom[0] = EE->LoadTexture("basehaste/models/weaphits/rlboom/rlboom_1.jpg", Pixelformat_RGBA8888);
	boom[1] = EE->LoadTexture("basehaste/models/weaphits/rlboom/rlboom_2.jpg", Pixelformat_RGBA8888);
	boom[2] = EE->LoadTexture("basehaste/models/weaphits/rlboom/rlboom_3.jpg", Pixelformat_RGBA8888);
	boom[3] = EE->LoadTexture("basehaste/models/weaphits/rlboom/rlboom_4.jpg", Pixelformat_RGBA8888);
	boom[4] = EE->LoadTexture("basehaste/models/weaphits/rlboom/rlboom_5.jpg", Pixelformat_RGBA8888);
	boom[5] = EE->LoadTexture("basehaste/models/weaphits/rlboom/rlboom_6.jpg", Pixelformat_RGBA8888);
	boom[6] = EE->LoadTexture("basehaste/models/weaphits/rlboom/rlboom_7.jpg", Pixelformat_RGBA8888);
	boom[7] = EE->LoadTexture("basehaste/models/weaphits/rlboom/rlboom_8.jpg", Pixelformat_RGBA8888);


	smoke[0] = EE->LoadTexture("basehaste/gfx/smoke01_na.tga", Pixelformat_RGBA8888);
	smoke[1] = EE->LoadTexture("basehaste/gfx/smoke02_na.tga", Pixelformat_RGBA8888);
	smoke[2] = EE->LoadTexture("basehaste/gfx/smoke03_na.tga", Pixelformat_RGBA8888);
	smoke[3] = EE->LoadTexture("basehaste/gfx/smoke04_na.tga", Pixelformat_RGBA8888);
	smoke[4] = EE->LoadTexture("basehaste/gfx/smoke05_na.tga", Pixelformat_RGBA8888);
	smoke[5] = EE->LoadTexture("basehaste/gfx/smoke06_na.tga", Pixelformat_RGBA8888);
	smoke[6] = EE->LoadTexture("basehaste/gfx/smoke07_na.tga", Pixelformat_RGBA8888);
	smoke[7] = EE->LoadTexture("basehaste/gfx/smoke08_na.tga", Pixelformat_RGBA8888);
	smoke[8] = EE->LoadTexture("basehaste/gfx/smoke09_na.tga", Pixelformat_RGBA8888);
	smoke[9] = EE->LoadTexture("basehaste/gfx/smoke10_na.tga", Pixelformat_RGBA8888);

	explode = EE->LoadSound(String("basehaste/sound/weapons/rocket/rocklx1a.wav"), FALSE);

	LoadMesh(String("grenade.obj"));
	EE->AttachToWorld(this);
//	Link();
}

EntRocket::EntRocket()
{
	EntRocket(NULL);
}

EntRocket::~EntRocket()
{
	EE.DestroyEntity(light);
	EE.DetachFromWorld(this);
}

void EntRocket::Init()
{
	light = EE.SpawnEntity("light", 0);
	light->Init();

	Vector3f	f, r, u;
	AngleVectors(angles, f, r, u);

	velocity = f * 1000;

	last_smoke_trail = origin;
}

void EntRocket::Think(void)
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
		light->SetOrigin(origin);

		target = origin + velocity * EE.Frametime();
		EE.trace.TraceBox(origin, target, mins, maxs);

		if(EE.trace.GetFraction() != 1.0f && !exploded)
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

			//make boom
			LoadSpriteAnimation(boom, 8, 8, origin, Vector2f(60, 60));

			//play sound
			PlaySound(explode);

			exploded = TRUE;

			return;
		}

		SetOrigin(EE.trace.GetCollisionPoint());

		//smoke trails
		if((last_smoke_trail - origin).length() > 10)
		{
			float	j;

//			for(j = 0; j < (last_smoke_trail - origin).length(); j += 10)
//				LoadSprite(smoke[0], last_smoke_trail + j * (last_smoke_trail - origin).normalize(), Vector2f(80, 80));

			last_smoke_trail = origin;
//			LoadSpriteAnimation(smoke, 10, 10, origin, Vector2f(80, 80));
			LoadSprite(smoke[0], origin, Vector2f(80, 80));
		}
	}

	spritecache->age(EE.Frametime() * 110);

	time += EE.Frametime();
	spritecache->time(time);

	ApplyEntityTransformToModel();
	EE.Link(this);
}

void EntRocket::SetProperty(String &key, String &value)
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
}

void EntRocket::GetProperty(String &key, String &value)
{
	Entity::GetProperty(key, value);
}

EXPORT_ENTITY(EntRocket)
