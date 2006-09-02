#include "common.h"
#include "EntLib/entity.h"

#include "ent_api.h"

//ION stuff
static Videodevice	*vd;
static Draw2DCache	*dcache;
static Audioserver	*audioserver;

//HDM demos
static int			demo_state = DEMO_STATE_NONE;

static ofstream		os;
static ifstream		is;

static Timer		timer;

//global map stuff
Q3BSP				*bspmap;
Entity				*player;
Simplespace			*skybox = NULL;

string				mapname;

static Timer		g_timer;
static float		g_frametime, g_last_frametime;;

//impending map change/restart flags; these actions should be performed in G_Frame only
static bool			new_map = FALSE;
static bool			map_restart = FALSE;

bool				done = FALSE;

std::map <int, std::list<Entity*> >		cluster_entities;
std::list <Entity*>						linked_entities;
std::list <Event>						events;
std::list <Entity*>						tb_destroyed_entities;
std::map <int, Entity*>					model_entities;

//cmds
void cmd_map(void);
void cmd_map_restart(void);

void cmd_record(void);
void cmd_play(void);
void cmd_stop(void);

void cmd_vertexstorage(void);
void cmd_mipmaps(void);
void cmd_texquality(void);
void cmd_wireframe(void);
void cmd_transparency(void);
void cmd_viewfrustumculling(void);

//TEMP
template <class T> static int CheckAABB(Vector3f mins1, Vector3f maxs1, const T mins2, const T maxs2)
{
	ion_int32	i;

	for(i = 0; i < 3; i++)
		if(mins1[i] > (float)maxs2[i] || maxs1[i] < (float)mins2[i])
			return false;

	return true;
}

void G_GetEntityList(std::list<Entity*> &elist, String &key, String &value)
{
	int		i;
	String	entity_value;

	for(i = 0; i < numWorldEntities; i++)
	{
		if(!WorldEntities[i])
			continue;
		
		WorldEntities[i]->GetProperty(key, entity_value);

		if(value == entity_value)
			elist.insert(elist.begin(), WorldEntities[i]);
	}
}

void G_ClearEntityLinks(void)
{
	cluster_entities.clear();
	linked_entities.clear();
	events.clear();
}

void G_LinkEntity(Q3BSP *bsp, Entity *ent)
{
	linked_entities.insert(linked_entities.begin(), ent);
}

void G_UnlinkEntity(Q3BSP *bsp, Entity *ent)
{
	linked_entities.remove(ent);
}

void G_ClusterEntities(std::list<Entity*> &elist, Entity *ent)
{
	int		i;

	for(i = 0; i < numWorldEntities; i++)
		if(WorldEntities[i] && WorldEntities[i] != ent)
			elist.insert(elist.begin(), WorldEntities[i]);
}

void G_ThrowEvent(Entity *entity, String name, int value, Entity *other)
{
	Event			event;
	events_iterator	ei;

	event.name = name;
	event.entity = entity;
	event.other = other;
	event.i_value = value;

	events.insert(events.end(), event);
	events.unique();
}

void G_DestroyEntity(Entity *ent)
{
	//append it to the end of the list, so this can be called from the "destroying loop" as well
	tb_destroyed_entities.push_back(ent);
}

void G_UpdateFrametime(void)
{
	bool	paused;
	cvars::getCvarBoolValue("paused", paused);

	g_last_frametime = (float)g_timer.difference() / (float)g_timer.frequency();

	g_timer.update();
	g_frametime = (float)g_timer.difference() / (float)g_timer.frequency();

	if(paused)
	{
		g_last_frametime = 0;
		g_frametime = 0;
	}
}

float G_Frametime(void)
{
	return g_frametime;
}

float G_LastFrametime(void)
{
	return g_last_frametime;
}

/*
========	
G_Frame
========
*/
void G_Frame(void)
{
	std::list <Entity*>			elist;
	linked_entities_iterator	ci, next_ci;
	linked_entities_iterator	ei;
	Entity						*entity1, *entity2;
	Vector3f					mins1, maxs1, mins2, maxs2, origin;
	events_iterator				events_i;

	int							i;
	int							num_entities;
	ion_int64					time;

#if 0
	if(new_map)
	{
		cmd_stop();
		G_EndMap();

		G_Intermission(cvars::getArg(1), TRUE);

		EngineExport EE(bspmap, m_pRenderer);
		G_LoadEntities(bspmap->entities(), &EE);

		Q3Shaderlist *pShaderlist=new Q3Shaderlist();
		pShaderlist->scanForShaders();
		bspmap->recalculateStaticVisibility();

		player = G_LoadEntity("Player", &EE);
		player->Init();

		if(!player)
		{
			logstream() << "player entity can not be loaded\n";

			G_EndMap();
			return;
		}

		player->SetCamera(*m_pCamera);
		player->Think();

		G_Intermission(mapname, FALSE);

		if(C_IsActive())
			cvars::execString("toggle_console");

		demo_state = DEMO_STATE_NONE;

		cvars::execString("stoptimer");
		cvars::execString("stoptimer");

		logstream() << mapname << " loaded\n";
	}
#endif

	//impending map_restart; reload all entities
	if(map_restart)
	{		
		if(!bspmap || !bspmap->isValid())
		{
			C_Print("map not loaded\n");
			return;
		}
/*
		delete bspmap;
		bspmap = new Q3BSP("maps/" + mapname + ".bsp", *m_pRenderer, NULL, 1.0f);
*/
		G_Intermission(mapname, TRUE);

		PH_DestroyBspModels();

		G_FreeEntities();
		EngineExport EE(bspmap, m_pRenderer, audioserver);
		G_LoadEntities(bspmap->entities(), &EE);
		
		player = G_LoadEntity("Player", &EE);
		player->Init();

		player->SetCamera(*m_pCamera);
		player->Think();

		cvars::execString("stoptimer");
		cvars::execString("stoptimer");

		//FIXME
/*
		for(i = 0; i < numWorldEntities; i++)
			if(BaselineEntities[i] && WorldEntities[i])
				*(WorldEntities[i]) = *(BaselineEntities[i]);
*/
		tb_destroyed_entities.clear();
		map_restart = FALSE;
		
		cvars::setCvarIntValue("fov", 90);
	
		return;
	}

	//delete all entities in the to be destroyed list
	for(ci = tb_destroyed_entities.begin(); ci != tb_destroyed_entities.end(); ci++)
	{
		entity1 = *ci;

		if(!entity1)
			continue;

		logstream() << "INDEX: " << entity1->GetEntityIndex() << "\n";
		logstream() << "CLASS: " << entity1->GetClassName() << "\n";

		WorldEntities[entity1->GetEntityIndex()] = NULL;
		delete entity1;
	}

	tb_destroyed_entities.clear();

	//clear the entity links from previous frame
	G_ClearEntityLinks();

	//update physics
	PH_Frame();

	//let everything Think()
	player->Think();
	for(i = 0; i < numWorldEntities; i++)
		if(WorldEntities[i] && WorldEntities[i] != player)
			WorldEntities[i]->Think();

	//only entities changing their position should be linked
	for(ei = linked_entities.begin(); ei != linked_entities.end(); ei++)
	{
		entity1 = *ei;
		G_ClusterEntities(elist, entity1);

		entity1->GetAABB(mins1, maxs1);
		origin = entity1->GetOrigin();

		mins1 += origin;
		maxs1 += origin;
	
		for(ci = elist.begin(); ci != elist.end(); ci++)
		{
			entity2 = *ci;

			if(entity2->GetModelNum() != -1)
				continue;

			entity2->GetAABB(mins2, maxs2);
			origin = entity2->GetOrigin();

			mins2 += origin;
			maxs2 += origin;

			if(CheckAABB<Vector3f>(mins1, maxs1, mins2, maxs2))
			{
				entity1->Touch(*entity2);
				entity2->Touch(*entity1);
			}
		}
	}

	//throw pending events
	for(events_i = events.begin(); events_i != events.end(); events_i++)
	{
		if((*events_i).name == "touch_model")
		{
			if(model_entities.find((*events_i).i_value) != model_entities.end())
			{
				entity2 = model_entities[(*events_i).i_value];

				(*events_i).entity->Touch(*entity2);
				entity2->Touch(*(*events_i).entity);
			}
		}
		else
		{
			(*events_i).entity->Event((*events_i).name, (*events_i).other);
		
			if((*events_i).other != NULL)
				(*events_i).other->Event((*events_i).name, (*events_i).entity);
		}
	}

	//demo recording/playback handling
	switch(demo_state)
	{
	case DEMO_STATE_RECORDING:
		timer.update();

		os << (ion_int64)(timer.lastTimeRel() * 10000.0f / timer.frequency()) << "\n";
		os << (int)linked_entities.size() << "\n";

		for(ei = linked_entities.begin(); ei != linked_entities.end(); ei++)
		{
			os << (int)entity1->GetEntityIndex() << "\n";
			(*ei)->Write(os);
		}
		break;

	case DEMO_STATE_PLAYING:
		timer.update();

		do
		{
			is >> time;
			if(time == -1)
			{
				cmd_stop();
				break;
			}

			is >> num_entities;
			for(i = 0; i < num_entities; i++)
			{
				int		ent_num;

				is >> ent_num;

				if(WorldEntities[ent_num])
					WorldEntities[ent_num]->Read(is);
			}
		}while(time < (timer.lastTimeRel() * 10000.0f / timer.frequency()));

		timer.update();
		while((timer.lastTimeRel() * 10000.0f / timer.frequency())  < time)
			timer.update();

		break;

	default:
		break;
	}
}

/*
=======
CMDS
=======
*/

void cmd_givew(void)
{
	//TODO
}

void cmd_mainmenu(void)
{
	G_EndMap();
	UI_ActivateMenu("asmenu", TRUE);
}

void cmd_quit(void)
{
	done = TRUE;
}

int vstoragemode=1;
 
void cmd_vertexstorage(void)
{
	switch (vstoragemode) {
		case 1:vstoragemode=2; break;
		case 2:vstoragemode=4; break;
		case 4:vstoragemode=1; break;
	}
 
	vd->deviceState(1,vstoragemode);
	bspmap->updateLightGeometry();
}
 
int texquality=1;
bool mipmaps=true;
 
void cmd_mipmaps(void)
{
	unsigned int i;
 
	mipmaps=!mipmaps;
	switch (texquality) {
		case 0:
			for (i=0;i<2;++i) {
				vd->samplerstageParameterInt(i,Samplerstageparameter_Minfilter,Texfilter_Point);
				vd->samplerstageParameterInt(i,Samplerstageparameter_Mipfilter,mipmaps ? Texfilter_Point : Texfilter_None);
				vd->samplerstageParameterInt(i,Samplerstageparameter_Magfilter,Texfilter_Point);
			}
			break;
		case 1:
			for (i=0;i<2;++i) {
				vd->samplerstageParameterInt(i,Samplerstageparameter_Minfilter,Texfilter_Linear);
				vd->samplerstageParameterInt(i,Samplerstageparameter_Mipfilter,mipmaps ? Texfilter_Point : Texfilter_None);
				vd->samplerstageParameterInt(i,Samplerstageparameter_Magfilter,Texfilter_Linear);
			}
			break;
	}
}
 
void cmd_texquality(void)
{
	unsigned int i;
 
	texquality=1-texquality;
	switch (texquality) {
		case 0:
			for (i=0;i<2;++i) {
				vd->samplerstageParameterInt(i,Samplerstageparameter_Minfilter,Texfilter_Point);
				vd->samplerstageParameterInt(i,Samplerstageparameter_Mipfilter,mipmaps ? Texfilter_Point : Texfilter_None);
				vd->samplerstageParameterInt(i,Samplerstageparameter_Magfilter,Texfilter_Point);
			}
			break;
		case 1:
			for (i=0;i<2;++i) {
				vd->samplerstageParameterInt(i,Samplerstageparameter_Minfilter,Texfilter_Linear);
				vd->samplerstageParameterInt(i,Samplerstageparameter_Mipfilter,mipmaps ? Texfilter_Point : Texfilter_None);
				vd->samplerstageParameterInt(i,Samplerstageparameter_Magfilter,Texfilter_Linear);
			}
			break;
	}
}
 
void cmd_wireframe(void)
{
	m_pRenderer->wireframe(!m_pRenderer->wireframe());
}
 
void cmd_transparency(void)
{
	m_pRenderer->translucency(!m_pRenderer->translucency());
}
 
void cmd_viewfrustumculling(void)
{
	bspmap->useFrustumCulling(!bspmap->usesFrustumCulling());
}

string	maplist[3] = {"map1", "haste0104s", "zero152"};

void cmd_nextmap(void)
{
	int		i;

	for(i = 0; i < 3; i++)
		if(mapname == maplist[i])
			break;

	i++;

	if(i >= 3)
		i = 0;

	cvars::execString("map " + maplist[i]);
}

/*
=======
G_Init
=======
*/
void G_Init(Videodevice &videodevice, Audioserver &as)
{
	Localfile	lf;

	//register static entities
	G_RegisterEntity("worldspawn", GetEntityRefAPIEntity);
	G_RegisterEntity("target_position", GetEntityRefAPIEntity);
	G_RegisterEntity("Player", GetEntityRefAPIPlayer);
	G_RegisterEntity("light", GetEntityRefAPIEntLight);
	G_RegisterEntity("trigger_push", GetEntityRefAPIEntTriggerPush);
	G_RegisterEntity("trigger_exit", GetEntityRefAPIEntTriggerExit);
	G_RegisterEntity("trigger_restart", GetEntityRefAPIEntTriggerRestart);
	G_RegisterEntity("trigger_timer", GetEntityRefAPIEntTriggerTimer);
	G_RegisterEntity("weapon_rocketlauncher", GetEntityRefAPIEntWeaponRocketlauncher);
	G_RegisterEntity("weapon_grenadelauncher", GetEntityRefAPIEntWeaponGrenadelauncher);
	G_RegisterEntity("ent_rocket", GetEntityRefAPIEntRocket);
	G_RegisterEntity("ent_grenade", GetEntityRefAPIEntGrenade);
	G_RegisterEntity("info_player_deathmatch", GetEntityRefAPIEntInfoPlayerDeathmatch);
	G_RegisterEntity("phys_model", GetEntityRefAPIEntPhysModel);
	G_RegisterEntity("func_static", GetEntityRefAPIEntPhysModel);
	G_RegisterEntity("item_haste", GetEntityRefAPIEntHaste);
	G_RegisterEntity("func_fovwarp", GetEntityRefAPIEntFuncFovwarp);

	cvars::addCmd("mainmenu", cmd_mainmenu);
	cvars::addCmd("nextmap", cmd_nextmap);
	cvars::addCmd("quit", cmd_quit);

	cvars::addCmd("map", cmd_map);
	cvars::addCmd("maprestart", cmd_map_restart);

	cvars::addCmd("record", cmd_record);
	cvars::addCmd("play", cmd_play);
	cvars::addCmd("stop", cmd_stop);

	cvars::addCmd("vertexstorage", cmd_vertexstorage);
	cvars::addCmd("mipmaps", cmd_mipmaps);
	cvars::addCmd("texquality", cmd_texquality);
	cvars::addCmd("wireframe", cmd_wireframe);
	cvars::addCmd("transparency", cmd_transparency);
	cvars::addCmd("viewfrustumculling", cmd_viewfrustumculling);

	//init physics
	PH_Init();

	memset(WorldEntities, 0, MAX_WORLD_ENTITIES * sizeof(Entity *));
	numWorldEntities = 0;

	vd = &videodevice;
	audioserver = &as;

	dcache = new Draw2DCache(videodevice, 20);
	if(!dcache->isValid())
		return;
}

/*
===============
G_Intermission
===============
*/
void G_Intermission(String name, bool loading)
{
	Localfile	lf;
	Texture2D	*pic;

	lf.open("levelshots/" + std::string(name) + ".jpg", "rb");

	if(!lf.streamIsOk())
	{
		lf.open("levelshots/" + std::string(name) + ".tga", "rb");

		if(!lf.streamIsOk())
		{
			lf.open("console/background2.png", "rb");
			if(!lf.streamIsOk())
				return;
		}
	}

	pic = load2DTex(*vd, "levelshots/" + std::string(name) + ".tga" , lf, video::Pixelformat_RGB888);
	lf.close();

	if(!pic)
	{
		logstream() << "G_Intermission: " << "could not find " << std::string(name) << ".tga" << "\n";
		return;
	}

	vd->begin();

	dcache->begin();
	dcache->addImage(*pic, &math::FloatRectangle(0, 0, 1, 1), math::FloatRectangle(0, 0, 1, 1), Draw2DCache::AddImageMode_ScreenNormalizedCoordinates);
	dcache->end();

	if(loading)
		C_DrawText("Loading", 0.43, 0.02);
	else
		C_DrawText("Click to continue...", 0.30, 0.02);
	
	vd->end();
	vd->update();

	if(!loading)
		pApp->waitForKey();

	delete pic;
}

/*
=========
G_EndMap
=========
*/
void G_EndMap(void)
{
	string	music = "basehaste/music/";

	G_FreeEntities();
//	R_FreeCachedModels();

//	R_FreeSkybox();

	if(bspmap)
		delete bspmap;
	
	bspmap = NULL;
	player = NULL;

	demo_state = DEMO_STATE_NONE;

	if(!C_IsActive())
		cvars::execString("toggle_console");

	cvars::setCvarIntValue("fov", 90);

/*
	//stop music
	if(mapname != "")
	{
		music += mapname + ".mp3";
		S_Stop(music);
	}
*/
}

/*
============
G_MapLoaded
============
*/
bool G_MapLoaded(void)
{
	return (bspmap != NULL) && bspmap->isValid();
}

/*
==============
G_GlobalFrame
==============
*/
void AngleVectorsQ(Vector3f angles, Camera &c)
{
	float fangx=angles[0]/100.0f;
	float fangy=angles[1]/100.0f;
	c.rotation(Quaternion::generateFromEulerAngles(fangx,0,0)*Quaternion::generateFromEulerAngles(0,fangy,0));
}

void R_DrawSkybox(void)
{
	m_pCamera->position(0, 0, 0);
	if(skybox)
		m_pRenderer->render(*m_pCamera, *skybox);
}

//omg CHANGE that name :)
void G_GlobalFrame(Vector3f angles)
{
	bool	paused;
	int		fov;

	cvars::getCvarBoolValue("paused", paused);

	G_UpdateFrametime();
	
	if(G_MapLoaded() && (!paused || G_Demostate() == DEMO_STATE_PLAYING))
	{
		if(player)
			player->SetAngles(Vector3f(angles));

		G_Frame();
	}

	//FIXME: after G_Frame, bspmap may not be valid anymore, fix that
	if(G_MapLoaded())
	{
		Vector3f	a;

		if(player)
			AngleVectorsQ(player->GetAngles(), *m_pCamera);

		R_DrawSkybox();
		m_pVideodevice->clear(Clear_Depthbuffer,Float4(0,0.0f,0.0f,0.0f));

		cvars::getCvarIntValue("fov", fov);
		m_pCamera->calculatePerspective(DEG2RAD(fov),1.0f,10000.0f);

		m_pCamera->position(player->GetOrigin() + Vector3f(0, 48, 0));
		m_pRenderer->render(*m_pCamera, *bspmap);
		m_pCamera->position(player->GetOrigin() - Vector3f(0, 48, 0));

		HUD_Draw();
		S_Update();
	}

	C_DrawConsole();
}

//FIXME: set the new_map flag and handle the rest in G_Frame
void cmd_map(void)
{
	string			music = "basehaste/music/";
	string			skyboxname = "mars/mars";

	list<Entity*>	worldspawn_list;
	Entity			*worldspawn;

	if(cvars::numArgs() < 2)
	{
		C_Print("Usage: map mapname\n");
		return;
	}

	cmd_stop();
	G_EndMap();

	mapname = "";
	map_restart = FALSE;
//	new_map = TRUE;
	
	G_Intermission(cvars::getArg(1), TRUE);

	logstream() << "Loading " << cvars::getArg(1) << "\n";

	bspmap = new Q3BSP("maps/" + std::string(cvars::getArg(1)) + ".bsp", *m_pRenderer, NULL, 1.0f);

	if(!bspmap || !bspmap->isValid())
	{		
		C_Print("%s.bsp could not be found\n", cvars::getArg(1).cstr());
		logstream() << cvars::getArg(1) << ".bsp could not be found\n";

		return;
	}

//	Color	ambient(0.5, 0.5, 0.5, 0.5);
	Color	ambient(0.3, 0.3, 0.3, 0.3);
//	Color	ambient(1, 1, 1, 1);
//	Color	ambient(0, 0, 0, 0);

	bspmap->ambientColor(ambient);

	mapname = cvars::getArg(1).STLstring();

	//init map physics
	PH_DestroyMap();
	PH_InitMap(bspmap);

	EngineExport EE(bspmap, m_pRenderer, audioserver);
	G_LoadEntities(bspmap->entities(), &EE);

	Q3Shaderlist *pShaderlist=new Q3Shaderlist();
	pShaderlist->scanForShaders();
//	bspmap->recalculateStaticVisibility();

	player = G_LoadEntity("Player", &EE);
	player->Init();

	if(!player)
	{
		logstream() << "player entity can not be loaded\n";

		G_EndMap();
		return;
	}

	player->SetCamera(*m_pCamera);
//	player->Think();

	G_GetEntityList(worldspawn_list, String("classname"), String("worldspawn"));
	if(!worldspawn_list.empty())
	{
		String	temp;

		worldspawn = *worldspawn_list.begin();

		worldspawn->GetProperty(String("skybox"), temp);
		if(temp != "")
			skyboxname = temp;
	}

	skybox = R_LoadSkybox("basehaste/env/" + skyboxname, *m_pRenderer);

	G_Intermission(cvars::getArg(1), FALSE);

	if(C_IsActive())
		cvars::execString("toggle_console");

	HUD_Activate();

	demo_state = DEMO_STATE_NONE;

	cvars::execString("stoptimer");
	cvars::execString("stoptimer");

	//play map music track
	music += mapname + ".mp3";
//	S_Play(music);
	S_Music(music);

	logstream() << cvars::getArg(1) << " loaded\n";
}

void cmd_map_restart(void)
{
	map_restart = TRUE;
}

void cmd_record(void)
{
	cmd_stop();

	if(cvars::numArgs() < 2)
	{
		C_Print("Usage: record demoname\n");
		return;
	}

	if(!bspmap || !bspmap->isValid())
	{
		C_Print("no map loaded\n");
		return;
	}

	os.open((string(cvars::getArg(1)) + ".hdm").c_str(), /*ofstream::binary | */ofstream::out | ofstream::trunc);

	if(os.good())
		demo_state = DEMO_STATE_RECORDING;

	os << DEMO_FILE_ID << " " << DEMO_FILE_VERSION << "\n";
	os << mapname << "\n";

	C_Print("recording to %s\n", cvars::getArg(1).cstr());

	timer.update();
	timer.adjustPivotTime();
}

void cmd_play(void)
{
	string	file_id;
	int		file_version;

	cmd_stop();

	if(cvars::numArgs() < 2)
	{
		C_Print("Usage: play demoname\n");
		return;
	}

	is.open((string(cvars::getArg(1)) + ".hdm").c_str(), /*ifstream::binary | */ifstream::in);

	if(!is.good())
	{
		C_Print("can not open %s\n", cvars::getArg(1).cstr());

		is.close();
		is.clear();
		return;
	}

	is >> file_id;
	if(file_id != DEMO_FILE_ID)
	{
		C_Print("%s is not a demo file\n", cvars::getArg(1).cstr());

		is.close();
		is.clear();
		return;
	}

	is >> file_version;
	if(file_version != DEMO_FILE_VERSION)
	{
		C_Print("wrong demo file version, %d should be %d\n", file_version, DEMO_FILE_VERSION);

		is.close();
		is.clear();
		return;
	}

	is >> mapname;

	C_Print("playing %s\n", cvars::getArg(1).cstr());

	cvars::execString("map " + mapname);

	if(mapname != "")
		demo_state = DEMO_STATE_PLAYING;

	timer.update();
	timer.adjustPivotTime();
}


void cmd_stop(void)
{
	switch(demo_state)
	{
	case DEMO_STATE_RECORDING:
		os << -1;
		os.close();

		C_Print("stopped demo\n");
		break;

	case DEMO_STATE_PLAYING:
		is.close();
		is.clear();
		G_EndMap();
		break;

	case DEMO_STATE_NONE:
//		C_Print("not recording or playing a demo\n");
		break;
	}

	demo_state = DEMO_STATE_NONE;
}

int G_Demostate(void)
{
	return demo_state;
}