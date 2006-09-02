#ifndef _G_ENTITY_H_
#define _G_ENTITY_H_

#include "common.h"

#define MAX_WORLD_ENTITIES		1024

//demos
#define DEMO_FILE_ID			"hastedemo"
#define DEMO_FILE_VERSION		5

#define DEMO_STATE_NONE			0
#define DEMO_STATE_RECORDING	1
#define DEMO_STATE_PLAYING		2

class Entity;
class EngineExport;

class Event
{
public:
	String	name;
	int		i_value;
	String	s_value;
	float	f_value;

	Entity	*entity, *other;

	bool operator==(Event &e)
	{
		if(name != e.name || i_value != e.i_value || s_value != e.s_value || f_value != e.f_value || entity != e.entity)
			return false;

		return true;
	}
};

typedef std::list <Entity*>::iterator	linked_entities_iterator;
typedef std::list <Event>::iterator		events_iterator;

#ifdef ENTITY_STATIC_BIND
#define ENTITY_INSTANCE(ent, EE) GetEntityRefAPI##ent(EE);

Entity *G_LoadEntity(const String name, EngineExport *EE);
void G_RegisterEntity(const String, Entity*(*GetEntityRefAPI)(EngineExport*));

#else
#define ENTITY_INSTANCE(ent, EE) GetEntityRefAPI(#ent, EE);

Entity *G_LoadEntity(const String name, EngineExport *EE);
void G_RegisterEntity(const String, Entity*(*GetEntityRefAPI)(EngineExport*));

#endif

void G_LoadEntities(String entstring, EngineExport *EE);
void G_GetEntityList(std::list<Entity*> &elist, String &key, String &value);

Entity *G_EntityByModel(int model_num);

void G_ClearEntityLinks(void);

void G_LinkEntity(Q3BSP *bsp, Entity *ent);
void G_UnlinkEntity(Q3BSP *bsp, Entity *ent);

void G_ClusterEntities(std::list<Entity*> &elist, Entity *ent);

void G_ThrowEvent(Entity *entity, String name, int value, Entity *other = NULL);


RenderableMesh	*R_CachedModel(std::string name, Renderer &renderer, Propertytable *model_props = NULL);
void			R_FreeCachedModels(void);

Sound			*R_CachedSound(std::string name, Audioserver &server, bool is_3D = FALSE);
void			R_FreeCachedSounds(void);

Simplespace *R_LoadSkybox(string name, Renderer &renderer);
void R_FreeSkybox(void);


#endif