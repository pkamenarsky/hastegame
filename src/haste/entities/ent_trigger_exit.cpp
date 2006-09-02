#include "ent_trigger_exit.h"

EntTriggerExit::EntTriggerExit(EngineExport *EE) : Entity(EE)
{
	ClassName = "trigger_exit";

	SetAABB(Vector3f(-50, -50, -50), Vector3f(50, 50, 50));

//	Link();
}

EntTriggerExit::EntTriggerExit()
{
}

EntTriggerExit::~EntTriggerExit()
{
}

void EntTriggerExit::Init()
{	
}

#define G_GRAVITY	800.0f

void EntTriggerExit::Touch(Entity &other)
{
	if(other.GetClassName() == "player")
		EE.C_ExecCommand(String("map ") + map);
}

void EntTriggerExit::SetProperty(String &key, String &value)
{
	Entity::SetProperty(key, value);

	if(key == "map")
		map = value;
}

void EntTriggerExit::GetProperty(String &key, String &value)
{
	Entity::GetProperty(key, value);
}

EXPORT_ENTITY(EntTriggerExit)
