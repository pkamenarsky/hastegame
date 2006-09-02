/*
==============================================================================
Lucifer Engine source code
Copyright (c) 2001 - 2002 by Morgoth

******************************************************************************
IMPORTANT:

This engine is under an absolutely secret devlopment. If you have a copy
of its source (e.g. you can read this), you are strongly encouraged to
destroy it as fast as you can. In case that you don't, you are breaking
applicable international laws, and your life will most likely be in
great danger.
******************************************************************************

ent_wrap.cpp
==============================================================================
*/

#define _INC_MATH
#include "../../common.h"

#include "ent_wrap.h"
#include "entity.h"

//#define MAX_WORLD_ENTITIES	512
//#define MAX_CLIENTS		16

game_import_t		gi;

Entity				*WorldEntities[2][MAX_WORLD_ENTITIES];
int					numWorldEntities[2] = {MAX_CLIENTS, MAX_CLIENTS};
int					num_clients[2] = {0, 0};

ENT_WRAP entity_t	*world_entities[2][MAX_WORLD_ENTITIES];
DLHANDLE			entity_libs[2][MAX_WORLD_ENTITIES];

static client_t		clients[2][MAX_CLIENTS];

ENT_WRAP float		frametime;

static int			entity_info = 0;

static int LoadWorldEntity(char *name, int net_role)
{
	char		lib_name[MAX_OS_PATH] = "./";		// "./entites/"
	char		classname[256] = "";

	DLHANDLE	lib_handle;
	Entity		*(*GetEntityRefAPI)(game_import_t*); //typedef this
	Entity		*edict;
	
	strcat(lib_name, name);
	strcat(lib_name, SYS_DLL_ENDING);
	
	if(!(lib_handle = LoadDLibrary(lib_name)))
	{
		if(entity_info) Error("EW_LoadWorldEntity: couldn't load %s\n", lib_name);
		return -1;
	}
	
	GetEntityRefAPI = (Entity*(*)(game_import_t*))LoadSymbol(lib_handle, "GetEntityRefAPI");
	
	gi.net_role = net_role;
/*	
	if(net_role == NR_SERVER) gi.frametime = srv_frametime;
	if(net_role == NR_CLIENT) gi.frametime = cl_frametime;
*/
	gi.frametime = &frametime;
	gi.num_entity = numWorldEntities[net_role];

	edict = GetEntityRefAPI(&gi);
//	edict->GetClassName(classname);
	
	//add to list
	if(numWorldEntities[net_role] >= MAX_WORLD_ENTITIES) return -1; //error handling

	WorldEntities[net_role][numWorldEntities[net_role]] = edict;
	world_entities[net_role][numWorldEntities[net_role]] = &edict->entity;
	world_entities[net_role][numWorldEntities[net_role]]->num_entity = numWorldEntities[net_role];

	entity_libs[net_role][numWorldEntities[net_role]] = lib_handle;

	return numWorldEntities[net_role]++;
}

void UnloadWorldEntity(void *ent)
{
	((Entity *)ent)->entity.inuse = FALSE;
}

static entity_t *LoadPlayerEntity(char *name, int net_role)
{
	char		lib_name[MAX_OS_PATH] = "./";		// "./entites/"
	char		classname[256] = "";

	DLHANDLE	lib_handle;
	Entity		*(*GetEntityRefAPI)(game_import_t*); //typedef this
	Entity		*edict;
	
	strcat(lib_name, name);
	strcat(lib_name, SYS_DLL_ENDING);
	
	if(!(lib_handle = LoadDLibrary(lib_name)))
	{
		if(entity_info) Error("EW_LoadWorldEntity: couldn't load %s\n", lib_name);
		return NULL;
	}
	
	GetEntityRefAPI = (Entity*(*)(game_import_t*))LoadSymbol(lib_handle, "GetEntityRefAPI");
	
	gi.net_role = net_role;
/*	
	if(net_role == NR_SERVER) gi.frametime = srv_frametime;
	if(net_role == NR_CLIENT) gi.frametime = cl_frametime;
*/
	gi.frametime = &frametime;
	gi.num_entity = num_clients[net_role];

	edict = GetEntityRefAPI(&gi);
//	edict->GetClassName(classname);
	
	//add to list
	if(num_clients[net_role] >= MAX_CLIENTS) return NULL; //error handling

	WorldEntities[net_role][num_clients[net_role]] = edict;
	world_entities[net_role][num_clients[net_role]] = &edict->entity;
	world_entities[net_role][num_clients[net_role]]->num_entity = num_clients[net_role];

	world_entities[net_role][num_clients[net_role]]->client = clients[net_role] + num_clients[net_role];

	entity_libs[net_role][num_clients[net_role]] = lib_handle;

	return world_entities[net_role][num_clients[net_role]++];
}

static char *SetEntityProperties(int entity_num, char *string, int net_role)
{
	Entity	*ent = WorldEntities[net_role][entity_num];
	
	if(entity_num < 0) return NULL;
	
	do
	{
		string = GetLineArgs(string);
		if(args[0][0] == '}') break;
		
		ent->SetProperty(args[0], args[1]);
	}while(string[0]);
	
	return	string;
}

static void LoadWorldEntities(int net_role)
{
	char	*line = re->GetEntityString();
	char	ent_name[256], *first_par;

	entity_info = CvarValue_i("entity_info");
	cprintf("loading %s world entities... ", net_role == NR_SERVER ? "NR_SERVER" : "NR_CLIENT");

	if(!line)
	{
		cprintf("ok, map has no entities.\n");
		return;
	}
	
	do
	{
		line = GetLineArgs(line);
		if(args[0][0] == '{') first_par = line;
		if(strcmp(args[0], "classname")) continue;

		strcpy(ent_name, "ent_");
		strcat(ent_name, args[1]);

		SetEntityProperties(LoadWorldEntity(ent_name, net_role), first_par, net_role);
	}while(line);

	cprintf(" ok, %d entities initialized\n", numWorldEntities[net_role]);
}

static void UnloadWorldEntities(int net_role)
{
	int		i;

	cprintf("unloading %s world entities... ", net_role == NR_SERVER ? "NR_SERVER" : "NR_CLIENT");
	
	for(i = 0; i < numWorldEntities[net_role]; i++)
	{
		CloseDLibrary(entity_libs[net_role][i]);
		
		WorldEntities[net_role][i] = NULL;
		world_entities[net_role][i] = NULL;
	}
	
	numWorldEntities[net_role] = MAX_CLIENTS;
	num_clients[net_role] = NULL;

	cprintf(" ok\n");
}

void GetEntities(EntityList *list, char *property, char *value, int net_role)
{
	int		i;
	char	v[256];

	for(i = MAX_CLIENTS; i < numWorldEntities[net_role]; i++)
	{
		if(world_entities[net_role][i]->flags & NR_INVISIBLE) continue;
		
		WorldEntities[net_role][i]->GetProperty(property, v);
		if(!strcmp(v, value)) list->Add(WorldEntities[net_role][i]);
	}
}

/*
=================
ENGINE INTERFACE
=================
*/

static void EntityThink(int ent_num, int net_role)
{
	if(WorldEntities[net_role][ent_num])
		WorldEntities[net_role][ent_num]->Think();
}

static char *GetArg(int num_arg)
{
	return (char *)&args[num_arg];
}

static int NumArgs(void)
{
	return num_args;
}

void *SpawnEntity(char *name, int net_role)
{
	return (void *)WorldEntities[net_role][LoadWorldEntity(name, net_role)];
}

ENT_WRAP void EW_RegisterEntityWrap(void)
{
	gi.SetNetVar = SetNetVar; 

	gi.CvarValue_i = CvarValue_i;
	gi.CvarValue_s = CvarValue_s;
	gi.GetCvar = GetCvar;

	gi.C_ExecCommand = Con_ExecCommand;

	gi.Trace = re->Trace;
	gi.AddCommand = AddCommand;

	gi.GetArg = GetArg;
	gi.NumArgs = NumArgs;

	gi.SpawnEntity = SpawnEntity;
	gi.DestroyEntity = UnloadWorldEntity;

	gi.GetEntities = (void (*)(void *, char *, char *, int))GetEntities;

	gi.frametime = &frametime;

	numWorldEntities[0] = MAX_CLIENTS;
	numWorldEntities[1] = MAX_CLIENTS;

	num_clients[0] = 0;
	num_clients[1] = 0;
/*
	for(i = 0; i < MAX_CLIENTS; i++)
		world_entities[i]->flags = NR_CLIENT | NR_NOT_USED;
*/
}

ENT_WRAP int EW_LoadWorldEntity(char *name, int net_role)
{
	return LoadWorldEntity(name, net_role);
}

ENT_WRAP entity_t *EW_LoadPlayerEntity(char *name, int net_role)
{
	return LoadPlayerEntity(name, net_role);
}

ENT_WRAP void EW_LoadWorldEntities(int net_role)
{
	LoadWorldEntities(net_role);
}

ENT_WRAP void EW_UnloadWorldEntities(int net_role)
{
	UnloadWorldEntities(net_role);
}

ENT_WRAP void EW_EntityThink(int ent_num, int net_role)
{
	EntityThink(ent_num, net_role);
}
