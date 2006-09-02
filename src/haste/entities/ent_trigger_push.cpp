#include "ent_trigger_push.h"

EntTriggerPush::EntTriggerPush(EngineExport *EE) : Entity(EE)
{
	ClassName = "trigger_push";

	SetAABB(Vector3f(-5, -5, -5), Vector3f(5, 5, 5));

//	Link();
}

EntTriggerPush::EntTriggerPush()
{
	Light(NULL);
}

EntTriggerPush::~EntTriggerPush()
{
}

void EntTriggerPush::Init()
{	
	std::list<Entity*>		info_player_start_elist;

	EE.GetEntityList(info_player_start_elist, String("targetname"), target_string);

	if(info_player_start_elist.empty())
		return;

	target = (Entity *)info_player_start_elist.front();
}

#define G_GRAVITY	800.0f

void EntTriggerPush::Touch(Entity &other)
{
//	Player	*player;
	float				height, time, dist;
	std::stringstream	sstr;

	if(!target)
		return;

	Vector3f	v = target->GetOrigin() - other.GetOrigin();

	height = v[1];
	time = (float)sqrt(height / (G_GRAVITY * 0.5));

	v[1] = 0;

	dist = v.length();
	v.normalize();
	v *= dist / time;
	v[1] = time * G_GRAVITY;

	sstr << "push" << " " << v[0] << " " << v[1] << " " << v[2];

	if(other.GetClassName() == "player")
	{
//		logstream() << "TOUCH: " << other.GetOrigin()[0] << " " << other.GetOrigin()[1] << " " << other.GetOrigin()[2] << "\n";
		EE.ThrowEvent_i(&other, sstr.str(), 0);

		//play sound
		EE.C_ExecCommand("s_play basehaste/sound/world/jumppad.wav");
	}
}

void EntTriggerPush::SetProperty(String &key, String &value)
{
	Entity::SetProperty(key, value);

	if(key == "target")
	{
//		logstream() << "TARGETNAME: " << value << "\n";
		target_string = value;
	}
}

void EntTriggerPush::GetProperty(String &key, String &value)
{
	Entity::GetProperty(key, value);
}

EXPORT_ENTITY(EntTriggerPush)
