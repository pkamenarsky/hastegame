#include "ent_weapon_rocketlauncher.h"

EntWeaponRocketlauncher::EntWeaponRocketlauncher(EngineExport *EE) : Entity(EE)
{
	ClassName = "weapon_rocketlauncher";

	SetAABB(Vector3f(-20, 0, -20), Vector3f(20, 20, 20));

	LoadMesh(String("gun3.obj"));
	EE->AttachToWorld(this);

	test = 0;

	float_time = 0;
	view_velocity = Vector3f(0, 0, 0);

//	Link();
}

EntWeaponRocketlauncher::EntWeaponRocketlauncher()
{
	EntWeaponRocketlauncher(NULL);
}

EntWeaponRocketlauncher::~EntWeaponRocketlauncher()
{
	EE.DetachFromWorld(this);
}

void EntWeaponRocketlauncher::Init()
{
	y = origin[1];
	test = 0;

	float_time = 0;
	view_velocity = Vector3f(0, 0, 0);
}

void EntWeaponRocketlauncher::Think(void)
{
	Entity::Think();

	time -= EE.Frametime();

	if(!commands.empty())
		if(*commands.begin() == "+attack" && parent && time <= 0)
		{
			int			rockets = 5;
			Entity		*ent;
			Vector3f	f, r, u;

			parent->GetIntProperty(String("rockets"), rockets);

			time = 1.0f;
			if(rockets > 0)
			{
				rockets--;
				parent->SetIntProperty(String("rockets"), rockets);

				AngleVectors(parent->GetAngles(), f, r, u);

				ent = EE.SpawnEntity("ent_rocket", 0);

				ent->SetOrigin(parent->GetOrigin() + r * 10 + f * 35 + Vector3f(0, 45, 0));
				ent->SetAngles(parent->GetAngles());

				ent->Init();

				//play sound
				EE.C_ExecCommand("s_play basehaste/sound/weapons/grenade/grenlf1a.wav");
			}
			else
				EE.C_ExecCommand("s_play basehaste/sound/weapons/noammo.wav");
		}

	ClearCommands();

	if(parent)
	{
		float_time -= EE.Frametime();

		Vector3f	a = parent->GetAngles();
		Vector3f	na(a[0], a[1] + 160, a[2]), v = parent->GetVelocity();

		//"activate" animation lol
		if(float_time > 0)
			na[0] -= 180 * float_time;

		view_velocity = (view_velocity + v * EE.Frametime() * 0.2) / 2;

		SetOrigin(parent->GetOrigin() + Vector3f(0, 48, 0) - view_velocity);
		SetAngles(na);
		ApplyEntityTransformToModel();
		return;
	}

	SetAngles(angles - Vector3f(0, EE.Frametime() * 300.0f, 0));

	float_time += EE.Frametime();
	SetOrigin(Vector3f(origin[0], y + sin(float_time * 8) * 2.5, origin[2]));

	ApplyEntityTransformToModel();
}

void EntWeaponRocketlauncher::Touch(Entity &other)
{
	if(other.GetClassName() != "player")
		return;

	if(test)
		return;

/*
	if(!test)
	{
		Entity *light = EE.SpawnEntity(String("light"), 0);
		light->SetOrigin(origin);
		light->Init();
	}
*/
	test = 1;
//	EE.DetachFromWorld(this);

	LoadMesh(String("v_gun.obj"));
//	EE.AttachToWorld(this);

	other.SetBoolProperty(String("rocket_launcher"), TRUE);
	other.SetIntProperty(String("rockets"), 5);
	other.Attach(this, FALSE);

	float_time = 0.5;
	time = 0;

	//play sound
	EE.C_ExecCommand("s_play basehaste/sound/misc/am_pkup.wav");
}


EXPORT_ENTITY(EntWeaponRocketlauncher)
