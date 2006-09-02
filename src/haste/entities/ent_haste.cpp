#include "ent_haste.h"

EntHaste::EntHaste(EngineExport *EE) : Entity(EE)
{
	ClassName = "light";

	SetAABB(Vector3f(-20, -20, -20), Vector3f(20, 20, 30));
	LoadMesh(String("basehaste/models/items/silencer/silencer.obj"));
	EE->AttachToWorld(this);

	float_time = 0;

//	Link();
}

EntHaste::EntHaste()
{
	EntHaste(NULL);
}

EntHaste::~EntHaste()
{
	EE.DetachFromWorld(this);
}

void EntHaste::Init()
{
	y = origin[1];
}

void EntHaste::Think(void)
{
	Entity::Think();

	SetAngles(angles - Vector3f(0, EE.Frametime() * 300.0f, 0));

	float_time += EE.Frametime();
	SetOrigin(Vector3f(origin[0], y + sin(float_time * 8) * 2.5, origin[2]));

	ApplyEntityTransformToModel();
}

void EntHaste::Touch(Entity &other)
{
	other.SetBoolProperty(String("item_haste"), TRUE);

	EE.C_ExecCommand("s_play basehaste/sound/items/haste.wav");
//	EE.C_ExecCommand("fov 100");

	EE.DetachFromWorld(this);
	EE.DestroyEntity(this);
}

EXPORT_ENTITY(EntHaste)
