#include "ent_func_fovwarp.h"

EntFuncFovwarp::EntFuncFovwarp(EngineExport *EE) : Entity(EE)
{
	ClassName = "light";
	touched = FALSE;

	SetAABB(Vector3f(-50, -50, -50), Vector3f(50, 50, 50));
//	Link();
}

EntFuncFovwarp::EntFuncFovwarp()
{
	EntFuncFovwarp(NULL);
}

EntFuncFovwarp::~EntFuncFovwarp()
{
}

void EntFuncFovwarp::Init()
{
	std::list<Entity*>		info_player_start_elist;

	EE.GetEntityList(info_player_start_elist, String("targetname"), target_string);

	if(info_player_start_elist.empty())
		return;

	target = (Entity *)info_player_start_elist.front();
}

void EntFuncFovwarp::Think(void)
{
	Entity::Think();
}

void EntFuncFovwarp::Touch(Entity &other)
{
	stringstream	sstr;
	Vector3f		to;

	if(!target || touched || other.GetClassName() != "player")
		return;

	touched = TRUE;
	to = target->GetOrigin();
	
	sstr << "fovwarp" << " " << origin[0] << " " << origin[1] << " " << origin[2] << " " << to[0] << " " << to[1] << " " << to[2];

	EE.ThrowEvent_i(&other, sstr.str(), 0);
}

void EntFuncFovwarp::SetProperty(String &key, String &value)
{
	Entity::SetProperty(key, value);

	if(key == "target")
		target_string = value;
}

void EntFuncFovwarp::GetProperty(String &key, String &value)
{
	Entity::GetProperty(key, value);
}

EXPORT_ENTITY(EntFuncFovwarp)
