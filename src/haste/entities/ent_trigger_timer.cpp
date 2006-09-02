#include "ent_trigger_timer.h"

EntTriggerTimer::EntTriggerTimer(EngineExport *EE) : Entity(EE)
{
	ClassName = "trigger_timer";

	SetAABB(Vector3f(-50, -50, -50), Vector3f(50, 50, 50));

	state = 0;
//	Link();
}

EntTriggerTimer::EntTriggerTimer()
{
}

EntTriggerTimer::~EntTriggerTimer()
{
}

void EntTriggerTimer::Init()
{	
}

#define G_GRAVITY	800.0f

void EntTriggerTimer::Touch(Entity &other)
{
	if(other.GetClassName() != "player")
		return;

	if(action == "start" && state != 1)
	{
		state = 1;
		EE.C_ExecCommand("starttimer");
	}
	else if(action == "stop" && state != 2)
	{
		state = 2;
		EE.C_ExecCommand("stoptimer");
	}
}

void EntTriggerTimer::SetProperty(String &key, String &value)
{
	Entity::SetProperty(key, value);

	if(key == "action")
		action = value;
}

void EntTriggerTimer::GetProperty(String &key, String &value)
{
	Entity::GetProperty(key, value);
}

EXPORT_ENTITY(EntTriggerTimer)
