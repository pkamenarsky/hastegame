#include "ent_trigger_restart.h"

EntTriggerRestart::EntTriggerRestart(EngineExport *EE) : Entity(EE)
{
	ClassName = "trigger_restart";
//	Link();
}

EntTriggerRestart::EntTriggerRestart()
{
}

EntTriggerRestart::~EntTriggerRestart()
{
}

void EntTriggerRestart::Init()
{	
}

void EntTriggerRestart::Touch(Entity &other)
{
	if(other.GetClassName() == "player")
		EE.C_ExecCommand(String("maprestart"));
}

void EntTriggerRestart::SetProperty(String &key, String &value)
{
	Entity::SetProperty(key, value);
}

void EntTriggerRestart::GetProperty(String &key, String &value)
{
	Entity::GetProperty(key, value);
}

EXPORT_ENTITY(EntTriggerRestart)
