//#include "eexport.h"
#include "EntLib/entity.h"

EngineExport::EngineExport(Q3BSP *v_bsp, Renderer *v_renderer, Audioserver *v_audioserver)
{
	trace.SetBSP(v_bsp);
	bsp = v_bsp;
	renderer = v_renderer;
	audioserver = v_audioserver;
}
	
EngineExport::EngineExport(void)
{
	trace.SetBSP(NULL);
}

void EngineExport::C_Print(String fmt, ...)
{
	va_list vl;
	char	str[1021];
	char	tmp[1024];

	strcpy(tmp, fmt.cstr());
	
	va_start(vl, tmp);
	
	vsprintf(str, fmt.cstr(), vl);
	::C_PrintString(str);
	
	va_end(vl);
}

void EngineExport::C_DrawText(String text, int x, int y)
{
	::C_DrawText(text.cstr(), x, y);
}

void EngineExport::RegisterEntity(String name, Entity*(*GetEntityRefAPI)(EngineExport*))
{
	G_RegisterEntity(name.STLstring(), GetEntityRefAPI);
}

Entity *EngineExport::SpawnEntity(String name, int net_role)
{
	return G_LoadEntity(name, this);
}

void EngineExport::DestroyEntity(Entity *ent)
{
	G_DestroyEntity(ent);
}

void EngineExport::GetEntityList(std::list<Entity*> &elist, String &key, String &value)
{
	G_GetEntityList(elist, key, value);
}

void EngineExport::AddCommand(String name, void (*Cmd)(void))
{
	cvars::addCmd(name.STLstring(), Cmd);
}

void EngineExport::C_ExecCommand(String cmd)
{
	cvars::execString(cmd.STLstring());
}

String EngineExport::GetArg(int arg_num)
{
	return cvars::getArg(arg_num);
}

int EngineExport::NumArgs(void)
{
	return cvars::numArgs();
}

void EngineExport::UpdateFrametime(void)
{
//	timer.update();
//	frametime = (float)timer.difference() / (float)timer.frequency();//* 1000;
}

float EngineExport::Frametime(void)
{
//	return frametime;
	return G_Frametime();
}

void EngineExport::Link(Entity *entity)
{
	G_LinkEntity(bsp, entity);
}

void EngineExport::Unlink(Entity *entity)
{
	G_UnlinkEntity(bsp, entity);
}

void EngineExport::ThrowEvent_i(Entity *ent, String event, int arg, Entity *other)
{
	G_ThrowEvent(ent, event, arg, other);
}

void EngineExport::AddLight(Light *light, bool is_static)
{
	bsp->addLight(*light/*, is_static*/);
	
//	if(is_static)
//		bsp->recalculateStaticVisibility();
}

void EngineExport::RemoveLight(Light *light)
{
	bsp->removeLight(*light);
}

void EngineExport::AttachToWorld(Entity *entity, int flags)
{
	if((flags & EF_MODEL) && entity->GetModelNode())
		bsp->addRenderable(*entity->GetModelNode()/*, false*/);

	if((flags & EF_SPRITE) && entity->GetSpriteCache())
		bsp->addRenderable(*entity->GetSpriteCache()/*, false*/);

//	if(entity->GetMainNode())
//		bsp->addChild(*entity->GetMainNode());
}

void EngineExport::DetachFromWorld(Entity *entity, int flags)
{
	if((flags & EF_MODEL) && entity->GetModelNode())
		bsp->removeRenderable(*entity->GetModelNode());

	if((flags & EF_SPRITE) && entity->GetSpriteCache())
		bsp->removeRenderable(*entity->GetSpriteCache());

//	if(entity->GetMainNode())
//		bsp->removeChild(*entity->GetMainNode());
}

RenderableMesh *EngineExport::LoadMesh(String &name, Propertytable *pt)
{
	return R_CachedModel(name.STLstring(), *renderer, pt);
}

Sound *EngineExport::LoadSound(String &name, bool is_3D)
{
	return R_CachedSound(name, *audioserver, is_3D);
}

Texture2D *EngineExport::LoadTexture(const String &name, Pixelformat pixelformat)
{
	Localfile	lf;
	Texture2D	*tex;

	lf.open(name, "rb");

	if(!lf.streamIsOk())
		return NULL;

	tex = C_LoadImage(lf, pixelformat);
	lf.close();

	return tex;
}

Spritecache *EngineExport::NewSpriteCache(int num_sprites)
{
	return new Spritecache("sprites", *renderer, num_sprites);
}
