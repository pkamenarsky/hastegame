/*
==============================================================================
ENTITY.CPP

abstract entity class implementation
==============================================================================
*/

#include <stdlib.h>
#include <string.h>

#include "../common.h"
#include "entity.h"

Entity::Entity(void)
{
//	EE = NULL;

	NetRole = NR_NONET;
	ClassName = "default_entity";

	model_num = -1;

	parent = NULL;

	mesh = NULL;
	node = NULL;

	spritecache = NULL;

	velocity = old_velocity = Vector3f(0, 0, 0);
	transform.setIdentity();

	group = new Group("entity");

	voice = NULL;
}

Entity::Entity(EngineExport *v_EE)
{
	EE = *v_EE;

	NetRole = NR_NONET;
	ClassName = "default_entity";
	
	model_num = -1;

	parent = NULL;

	mesh = NULL;
	node = NULL;

	spritecache = NULL;
	
	velocity = Vector3f(0, 0, 0);
	transform.setIdentity();

	group = new Group("entity");

	voice = NULL;
}

Entity::Entity(String &classname, EngineExport *v_EE)
{	
	EE = *v_EE;

	NetRole = NR_NONET;
	ClassName = classname;

	spritecache = NULL;
	
	group = new Group("entity");
}

void Entity::Init(void)
{
}

Entity::~Entity(void)
{
//FIXME
//	if(node) delete node;
	if(group) delete group;
//	if(sound) delete sound; //??????
}

void Entity::SetAABB(Vector3f &v_mins, Vector3f &v_maxs)
{
	mins = v_mins;
	maxs = v_maxs;
}

void Entity::GetAABB(Vector3f &v_mins, Vector3f &v_maxs)
{
	v_mins = mins;
	v_maxs = maxs;
}

void Entity::SetOrigin(Vector3f &v_origin)
{
	origin = v_origin;

	transform[12] = origin[0];
	transform[13] = origin[1];
	transform[14] = origin[2];

	if(node)
		node->position(origin);
}

void Entity::SetCenter(Vector3f &v_center)
{
	center = v_center;
}

void Entity::SetVelocity(Vector3f &v_velocity)
{
	velocity = v_velocity;
}

void Entity::SetAngles(Vector3f &v_angles)
{
	Quaternion	q;

	angles = v_angles;
}

void Entity::SetModelTransform(Matrix4f &v_transform)
{
	transform = v_transform;

	if(node)
	{
		//model HACK
		transform[12] += center[0];
		transform[13] += center[1];
		transform[14] += center[2];

		origin[0] = transform[12];
		origin[1] = transform[13];
		origin[2] = transform[14];

		node->extractLocalTransformFrom(transform);

		transform[12] -= center[0];
		transform[13] -= center[1];
		transform[14] -= center[2];
	}
}

void Entity::ApplyEntityTransformToModel(void)
{
	Quaternion	q;
	Vector3f	f, r, u;
	Matrix4f	fix;

	AngleVectorsQ2(angles, q);
	transform.fromQuaternion(q);

	SetMeshOrigin(origin + center);

	if(node)
		node->extractLocalTransformFrom(transform);
}

void Entity::SetMeshOrigin(Vector3f &v_origin)
{
	transform[12] = v_origin[0];
	transform[13] = v_origin[1];
	transform[14] = v_origin[2];
}

void Entity::SetFlags(int flags)
{
//	entity.flags = flags;
}
void Entity::SetCamera(Camera &v_camera)
{
	camera = &v_camera;
}

Camera &Entity::GetCamera(void)
{
	return *camera;
}

Vector3f &Entity::GetOrigin(void)
{
	return origin;
}

Vector3f &Entity::GetCenter(void)
{
	return center;
}

Vector3f &Entity::GetVelocity(void)
{
	return velocity;
}

Vector3f &Entity::GetOldVelocity(void)
{
	return old_velocity;
}

Vector3f &Entity::GetAngles(void)
{
	return angles;
}

Matrix4f &Entity::GetModelTransform(void)
{
	return transform;
}

int Entity::GetFlags(void)
{
	return 0;//entity.flags;
}

String &Entity::GetClassName(void)
{
	return ClassName;
}

int Entity::GetModelNum(void)
{
	return model_num;
}

void Entity::SetEntityIndex(int index)
{
	entity_index = index;
}

int Entity::GetEntityIndex(void)
{
	return entity_index;
}

void Entity::LoadMesh(String &name)
{
//	if(mesh) delete mesh;
//	if(node) delete node;

	Propertytable	*pt = new Propertytable;
	pt->addRef();
	mesh = EE.LoadMesh(name, pt);

	if(!mesh)
		return;

	if(!node)
	{
		node = new RenderableMeshNode("tnode", *mesh);
		node->position(Vector3f(0, 0, 0));
		node->rotation(ion::math::Quaternion::generateFromRotAxisAndAngle(Vector3f(0,1,0),0));

		group->addChild(*node);
		group->position(origin);
		group->rotation(ion::math::Quaternion::generateFromRotAxisAndAngle(Vector3f(0,1,0),0));
	}
	else
		node->renderableMesh(*mesh);

	node->materialProperties(0, pt);
}

int Entity::LoadSprite(const String &name, const Vector3f &pos, const Vector2f &dim)
{
	if(spritecache)
	{
		Texture2D	*tex = EE.LoadTexture(name, Pixelformat_RGBA8888);

		if(tex)
//			spritecache->addSprite(pos, dim, tex, video::BlendFunc_Src_Alpha, video::BlendFunc_One_Minus_Src_Alpha);
			spritecache->addSprite(pos, dim, tex, video::BlendFunc_One, video::BlendFunc_One, 100);
	}

	//FIXME
	return 0;
}

int Entity::LoadSprite(Texture2D *tex, const Vector3f &pos, const Vector2f &dim)
{
	if(spritecache && tex)
	{
//		spritecache->addSprite(pos, dim, tex, video::BlendFunc_Src_Alpha, video::BlendFunc_One_Minus_Src_Alpha);
		spritecache->addSprite(pos, dim, tex, video::BlendFunc_One, video::BlendFunc_One, 30);
	}

	//FIXME
	return 0;
}

int	Entity::LoadSpriteAnimation(const String **names, int num, int fps, const Vector3f &pos, const Vector2f &dim)
{
	if(spritecache)
	{
		int			i;
		Texture2D	*tex[512];
		for(i = 0; i < num; i++)
			tex[i] = EE.LoadTexture(*names[i], Pixelformat_RGBA8888);

		if(tex)
//			spritecache->addSprite(pos, dim, tex, video::BlendFunc_Src_Alpha, video::BlendFunc_One_Minus_Src_Alpha);
			spritecache->addSprites(pos, dim, tex, num, fps, video::BlendFunc_One, video::BlendFunc_One, 100);
	}

	//FIXME
	return 0;
}

int	Entity::LoadSpriteAnimation(Texture2D **tex, int num, int fps, const Vector3f &pos, const Vector2f &dim)
{
	if(spritecache && tex)
	{
//		spritecache->addSprite(pos, dim, tex, video::BlendFunc_Src_Alpha, video::BlendFunc_One_Minus_Src_Alpha);
		spritecache->addSprites(pos, dim, tex, num, fps, video::BlendFunc_One, video::BlendFunc_One, 100);
	}

	//FIXME
	return 0;
}

void Entity::Attach(Entity *other, bool attach_node)
{
	if(attach_node)
		group->addChild(*other->GetMainNode());

	children.insert(children.begin(), other);
	children.unique();

	other->parent = this;
}

void Entity::Detach(Entity *other)
{
	node->removeChild(*other->GetMainNode());

	children.remove(other);
	other->parent = NULL;
}

RenderableMeshNode *Entity::GetModelNode(void)
{
	return node;
}

Spritecache *Entity::GetSpriteCache(void)
{
	return spritecache;
}

void Entity::PlaySound(Sound *s)
{
	voice = s->play(0.1, 0, &origin, &velocity, FALSE, FALSE);
}

Group *Entity::GetMainNode(void)
{
	return group;
}

void Entity::Think(void)
{
	EE.UpdateFrametime();

	old_velocity = velocity;

	if(voice)
	{
		voice->position(origin);
		voice->velocity(velocity);
	}
}

void Entity::Die(Entity &killer)
{
}

void Entity::Touch(Entity &other)
{
}

void Entity::Event(String &event, Entity *other)
{
}

void Entity::RecursiveCommand(String cmd)
{
	elist_iterator	li;

	commands.insert(commands.begin(), cmd);
	commands.unique();

	for(li = children.begin(); li != children.end(); li++)
		(*li)->RecursiveCommand(cmd);
}

void Entity::ClearCommands(void)
{
	commands.clear();
}

void Entity::SetProperty(String &key, String &value)
{

	if(key == "classname" && ClassName == "default_entity")
		ClassName = value;
	else if(key == "origin")
	{
		std::stringstream	sstr(value);
		Vector3f	v;
		
		sstr >> v[0] >> v[1] >> v[2];

		VecSwizzle(v);
		SetOrigin(v);
	}
	else if(key == "angle")
	{
		std::stringstream	sstr(value);
		float	v;
		
		sstr >> v;
		SetAngles(Vector3f(0, v, 0));
	}
	else if(key == "model")
	{
		std::stringstream	sstr(value.STLstring().substr(1));
		sstr >> model_num;
	}
	else if(key == "targetname")
		targetname = value;
	else
		properties[key] = value;

}

void Entity::GetProperty(String &key, String &value)
{
	value = "";

	if(key == "origin")
	{
		Vector3f origin = GetOrigin();
//		sprintf(value.cstr(), "%d %d %d", (int)origin[0], (int)origin[1], (int)origin[2]);
	}
	else if(key == "classname")
		value = ClassName;
	else if(key == "targetname")
		value = targetname;
	else if(key == "angle")
	{
		stringstream	sstr;

		sstr << angles[1];

		value = sstr.str();
	}
	else
		if(properties.find(key) != properties.end())
			value = properties[key];
}

void Entity::SetIntProperty(String &key, int value)
{
	stringstream	sstr;

	sstr << value;
	SetProperty(key, String(sstr.str()));
}

void Entity::GetIntProperty(String &key, int &value)
{
	String	str_value;
	GetProperty(key, str_value);

	stringstream	sstr(str_value);
	sstr >> value;
}

void Entity::SetBoolProperty(String &key, bool value)
{
	if(value)
		SetProperty(key, String("true"));
	else
		SetProperty(key, String("false"));
}

void Entity::GetBoolProperty(String &key, bool &value)
{
	String	str_value;
	GetProperty(key, str_value);

	if(str_value == "true")
		value = TRUE;
	else
		value = FALSE;
}

bool Entity::Write(ofstream &os)
{
	os << origin[0] << " ";
	os << origin[1] << " ";
	os << origin[2] << "\n";

	os << angles[0] << " ";
	os << angles[1] << " ";
	os << angles[2] << "\n";

	return true;
}

bool Entity::Read(ifstream &is)
{	is >> origin[0] >> origin[1] >> origin[2];
	is >> angles[0] >> angles[1] >> angles[2];

	return true;
}

void GetServerAPI(EngineExport *EE)
{
}

EXPORT_ENTITY(Entity)