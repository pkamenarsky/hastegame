/***************************************************************************
ENTITY.H
***************************************************************************/

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "../eexport.h"
#include "../trace.h"

#include "../common.h"

#ifdef WIN32
#define SYS_DLL_EXPORT __declspec(dllexport)
#endif

//netvar types
#define NV_RELIABLE		0
#define NV_UNRELIABLE	1

#define NV_BYTE			0
#define NV_SHORT		1
#define NV_LONG			2
#define NV_FLOAT		3
#define NV_DOUBLE		4

#define NV_VEC_FLOAT	5

#define NV_NUM_TYPES	6

//net roles
#define NR_CLIENT		0x00
#define NR_SERVER		0x01

#define NR_NONET		0x04
#define NR_SIMULATE		0x08
#define NR_PLAYER		0x10
#define NR_TEMP			0x20
#define NR_NOT_USED		0x40

#define NR_INVISIBLE	0x20

//events
#define EV_IDENTIFY		0x0
#define EV_THINK		0x1
#define EV_DIE			0x2
#define EV_TOUCH		0x3

class EngineExport;

typedef	list <Entity*>::iterator	elist_iterator;
typedef	list <string>::iterator		cmdlist_iterator;

/*
==================
 The Entity Class
==================
*/
class Entity
{
public:
	Entity(void);
	Entity(EngineExport *v_EE);
	Entity(String &classname, EngineExport *v_EE);

	virtual		~Entity(void);

	void		SetAABB(Vector3f &mins, Vector3f &maxs);
	void		GetAABB(Vector3f &mins, Vector3f &maxs);

	void		SetOrigin(Vector3f &origin);
	Vector3f	&GetOrigin(void);

	void		SetCenter(Vector3f &center);
	Vector3f	&GetCenter(void);

	void		SetVelocity(Vector3f &velocity);
	Vector3f	&GetVelocity(void);
	//need that for physics
	Vector3f	&GetOldVelocity(void);

	void		SetAngles(Vector3f &angles);
	Vector3f	&GetAngles(void);

	void		SetModelTransform(Matrix4f &v_transform);
	Matrix4f	&GetModelTransform(void);

	void		ApplyEntityTransformToModel(void);

	void		SetMeshOrigin(Vector3f &v_origin);

	void		SetFlags(ion_int32 flags);
	ion_int32	GetFlags(void);

	void		SetCamera(Camera &v_camera);
	Camera		&GetCamera(void);

	String		&GetClassName(void);

	int			GetModelNum(void);

	void		LoadMesh(String &name);
	int			LoadSprite(const String &name, const Vector3f &pos, const Vector2f &dim);
	int			LoadSprite(Texture2D *tex, const Vector3f &pos, const Vector2f &dim);

	int			LoadSpriteAnimation(const String **names, int num, int fps, const Vector3f &pos, const Vector2f &dim);
	int			LoadSpriteAnimation(Texture2D **tex, int num, int fps, const Vector3f &pos, const Vector2f &dim);

	void		Attach(Entity *other, bool attach_node);
	void		Detach(Entity *other);

	void		PlaySound(Sound *s);

	void		SetEntityIndex(int index);
	int			GetEntityIndex(void);

	RenderableMeshNode	*GetModelNode(void);
	Group				*GetMainNode(void);

	Spritecache	*GetSpriteCache(void);

	//VIRTUALS

	//this gets called after all map entities have been loaded
	virtual void	Init(void);

	virtual void	RecursiveCommand(String cmd);
	void			ClearCommands(void);

	virtual void	Event(String &event, Entity *other = NULL);

	virtual void	SetProperty(String &key, String &value);
	virtual void	GetProperty(String &key, String &value);

	virtual void	SetIntProperty(String &key, int value);
	virtual void	GetIntProperty(String &key, int &value);

	virtual void	SetBoolProperty(String &key, bool value);
	virtual void	GetBoolProperty(String &key, bool &value);

	virtual void	Think(void);
	virtual void	Die(Entity &killer);
	virtual void	Touch(Entity &other);
	
	//EVENT HANDLING
/*
============================================================
	void	ThrowEvent(int entity_num, char *event, int arg);
	void	ThrowEvent(int entity_num, int event, int arg);
	
	void	RegisterEvent(char *event, void (*func)(int));
	void	RegisterEvent(int event, void (*func)(int));

  virtual void	ProcessEvent(int event, int arg);
============================================================
*/	
	//I/O
	virtual bool	Write(ofstream &os);
	virtual bool	Read(ifstream &is);
	
protected:
	int			entity_index;

	int			NetRole;
	String		ClassName;

	String		targetname;

	Camera		*camera;

	int			model_num;
	
	int			frametime;

	Vector3f	mins, maxs;

	Vector3f	origin;
	Vector3f	center;
	Vector3f	velocity, old_velocity;
	Vector3f	angles;

	Matrix4f	transform;

	//all the rest :)
	map <string, string>	properties;

	//"virtual" children
	list <Entity* >		children;
	list <string >		commands;

	Entity				*parent;

	Spritecache			*spritecache;

	//models
	RenderableMeshNode	*node;
	RenderableMesh		*mesh;
	Group				*group;

	Voice				*voice;	
public:
	EngineExport	EE;

	Entity		*next, *prev;
};

void GetServerAPI(EngineExport *EE);

/*
==============
EXPORT MACROS
==============
*/

#ifdef ENTITY_STATIC_BIND
//entity interface export macro
#define EXPORT_ENTITY(ent)				\
	Entity *GetEntityRefAPI##ent(EngineExport *EE)	\
	{									\
		return new ent(EE);	\
	}
#define EXPORT_ENTITY_H(ent) Entity *GetEntityRefAPI##ent(EngineExport *EE);
#else
//entity interface export macro
#define EXPORT_ENTITY(ent)				\
	Entity *GetAPI(EngineExport *EE)	\
	{									\
		GetServerAPI(EE);				\
		return new ent(EE);	\
	}									\
										\
	extern "C" SYS_DLL_EXPORT Entity *GetEntityRefAPI(EngineExport *EE)	\
	{									\
		return GetAPI(EE);				\
	}
#define EXPORT_ENTITY_H(ent)
#endif

EXPORT_ENTITY_H(Entity)

#endif
