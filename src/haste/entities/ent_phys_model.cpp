#include "ent_phys_model.h"

EntPhysModel::EntPhysModel(EngineExport *EE) : Entity(EE)
{
	ClassName = "phys_model";

	model = "";
	physics = TRUE;
	rotation = Vector3f(0, 0, 0);

	SetAABB(Vector3f(-5, -5, -5), Vector3f(5, 5, 5));
//	LoadMesh(String("bla"));
//	EE->AttachToWorld(this);
}

EntPhysModel::EntPhysModel()
{
	EntPhysModel(NULL);
}

EntPhysModel::~EntPhysModel()
{
	if(model != "")
		EE.DetachFromWorld(this);
}

void EntPhysModel::Init()
{
//	model = "basehaste/models/barrel/barrel.obj";
//	model = "gun2.obj";

	if(model == "")
		return;

	LoadMesh(model);
	EE.AttachToWorld(this);

	if(physics)
		PH_AddBspModel(this);
	else
		ApplyEntityTransformToModel();

	//set old_velocity
	Think();
}

void EntPhysModel::Think(void)
{
	Entity::Think();

	velocity[1] -= 800 * EE.Frametime();
	velocity += add_velocity;

	add_velocity = Vector3f(0, 0, 0);

	if(!physics)
	{
		angles += rotation * EE.Frametime();
		ApplyEntityTransformToModel();
	}
}

void EntPhysModel::Event(String &event, Entity *other)
{
	std::stringstream	sstr(event);
	std::string			token;

	sstr >> token;
	
	if(token == "airpush")
	{
		Vector3f	p;
		sstr >> p[0] >> p[1] >> p[2];

		velocity += p;
	}
}

void EntPhysModel::Touch(Entity &other)
{
	if(other.GetClassName() != "player")
		return;

	Vector3f	f, r, u;
	float		amount = 0.0f;
	AngleVectors(other.GetAngles(), f, r, u);

//	add_velocity = -0.001 * amount * velocity + f * 200;
//	add_velocity[1] = 1000;
}

void EntPhysModel::SetProperty(String &key, String &value)
{
	Entity::SetProperty(key, value);

	if(key == "drawmodel")
		model = "basehaste/models/" + value + ".obj";
	else if(key == "physics")
		physics = (value == "1") ? TRUE : FALSE;
	else if(key == "rotation")
	{
		stringstream	sstr(value);

		sstr >> rotation[0];
		sstr >> rotation[1];
		sstr >> rotation[2];
	}
}

void EntPhysModel::GetProperty(String &key, String &value)
{
	Entity::GetProperty(key, value);
}

EXPORT_ENTITY(EntPhysModel)
