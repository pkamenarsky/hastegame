#ifndef _EEXPORT_H_
#define _EEXPORT_H_


#include "trace.h"
#include "g_entity.h"

#include <vector>
//#include "EntLib/entity.h"

//AttachToWorld/DetachFromWorld flags
#define EF_MODEL	0x1
#define EF_SPRITE	0x2
#define EF_ALL		(EF_MODEL | EF_SPRITE)

class Entity;

class EngineExport
{
public:
	Trace		trace;
	Q3BSP		*bsp;
	Renderer	*renderer;
	Audioserver	*audioserver;

	EngineExport(Q3BSP *v_bsp, Renderer *v_renderer, Audioserver *v_audioserver);
	EngineExport(void);

	void	C_Print(String fmt, ...);
	void	C_DrawText(String text, int x, int y);

	void	RegisterEntity(String name, Entity*(*GetEntityRefAPI)(EngineExport*));
//	cvar_t	GetCvar(char *name, char *default_value, int flags);

	void	AddCommand(String name, void (*Cmd)(void));
	void	C_ExecCommand(String cmd);
	
	char	*CvarValue_s(String name);
	int		CvarValue_i(String name);
	
	void	ThrowEvent_s(Entity *ent, String event, String arg, Entity *other = NULL);
	void	ThrowEvent_i(Entity *ent, String event, int arg, Entity *other = NULL);
	
	void	RegisterEvent_s(int ent_num, String event, void (*func)(int));
	void	RegisterEvent_i(int ent_num, int event, void (*func)(int));

	Entity	*SpawnEntity(String name, int net_role);
	void	DestroyEntity(Entity *ent);

	void	GetEntityList(std::list<Entity*> &elist, String &key, String &value);
	
	String	GetArg(int arg_num);
	int		NumArgs(void);
	
	void	UpdateFrametime(void);
	float	Frametime(void);

	void	Link(Entity *entity);
	void	Unlink(Entity *entity);

	void	AddLight(Light *light, bool is_static);
	void	RemoveLight(Light *light);

	void	AttachToWorld(Entity *entity, int flags = EF_ALL);
	void	DetachFromWorld(Entity *entity, int flags = EF_ALL);

	RenderableMesh	*LoadMesh(String &name, Propertytable *pt = NULL);
	Sound			*LoadSound(String &name, bool is_3D = FALSE);
	Texture2D		*LoadTexture(const String &name, Pixelformat pixelformat);

	Spritecache		*NewSpriteCache(int num_sprites);

protected:
	Timer	timer;
	float	frametime;
};

void VecSwizzle(Vector3f& v);

#endif