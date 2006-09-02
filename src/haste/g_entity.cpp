/*
==========================================================================================
HASTE

g_entity.cpp - entity loading functions
==========================================================================================
*/

#include "common.h"
#include "EntLib/entity.h"

#include "../video/video_all.hh"

//global world entities
Entity	*WorldEntities[MAX_WORLD_ENTITIES];
Entity	*BaselineEntities[MAX_WORLD_ENTITIES];
int		numWorldEntities;

//only defined if the entities are included statically in the binary
#ifdef ENTITY_STATIC_BIND

std::map <std::string, Entity*(*)(EngineExport*)> entity_func_array;

void G_RegisterEntity(const String name, Entity*(*GetEntityRefAPI)(EngineExport*))
{
	entity_func_array[name.STLstring()] = GetEntityRefAPI;
}

Entity *G_LoadEntity(const String name, EngineExport *EE)
{
	Entity	*ent;

	if(entity_func_array.find(name.STLstring()) != entity_func_array.end())
	{

		if(numWorldEntities >= MAX_WORLD_ENTITIES)
		{
			ent = NULL;
			logstream() << "G_LoadEntity: WARNING: numWorldEntities exceeded\n";
		}
		else
		{
			ent = entity_func_array[name.STLstring()](EE);
			WorldEntities[numWorldEntities] = ent;

			ent->SetEntityIndex(numWorldEntities++);
		}

		return ent;
	}
	else
	{
		logstream() << "G_LoadEntity: WARNING: entity " << name << " can not be loaded\n";
		return NULL;
	}
}

#else

void G_RegisterEntity(const String name, Entity*(*GetEntityRefAPI)(EngineExport*))
{
	//nop
}

Entity *G_LoadEntity(String name, EngineExport *EE)
{
	Dynobject	ent_lib(name.cstr());
	Entity		*(*GetEntityRefAPI)(EngineExport*); //typedef this

	if(!ent_lib.isValid())
	{
		logstream() << "ENTLIB\n";
		return NULL;
	}

	GetEntityRefAPI = (Entity*(*)(EngineExport*))ent_lib.function("GetEntityRefAPI");

	if(!GetEntityRefAPI)
	{
		logstream() << "FUNC\n";
		return NULL;
	}

	 return GetEntityRefAPI(EE);
}
#endif

/*
===============
G_LoadEntities

FIXME: baseline entities
===============
*/
void G_LoadEntities(String entstring, EngineExport *EE)
{
	std::stringstream	sstr(entstring.STLstring());
	std::string			token, key, value;

	std::string			keys[32], values[32];
	int					num_pairs;

	Entity				*ent, *base;

	int					i;

	numWorldEntities = 0;

	while(!sstr.eof())
	{
		sstr >> token;

		if(sstr.eof())
			break;

		if(token != "{")
		{
			logstream() << "G_LoadEntities: ERROR: missing '{'\n";
			return;
		}

		ent = NULL;
		num_pairs = 0;

		while(!sstr.eof())
		{
			sstr >> token;

			if(sstr.eof())
			{
				logstream() << "G_LoadEntities: ERROR: unexpected end of entstring\n";
				return;
			}

			if(token == "}")
				break;

			key = token.substr(1, token.length() - 2);

			sstr >> token;

			if(token[0] == '\"' && token[token.length() -1] != '\"')
			{
				std::string	temp;
	
				do
				{
					temp.erase();
					sstr >> temp;
					token += " " + temp;
				}while(temp[temp.length() - 1] != '\"');
			}

			value = token.substr(1, token.length() - 2);

			if(num_pairs < 32)
			{
				keys[num_pairs] = key;
				values[num_pairs++] = value;
			}

			if(key == "classname")
			{
				ent = G_LoadEntity(String(value), EE);
//				base = G_LoadEntity(String(value), EE);
			}
		}

		if(ent)
		{
			int		i;

			for(i = 0; i < num_pairs; i++)
			{
//				base->SetProperty(String(keys[i]), String(values[i]));
				ent->SetProperty(String(keys[i]), String(values[i]));
			}
		}
	}

	G_ClearEntityLinks();

	model_entities.clear();

	for(i = 0; i < numWorldEntities; i++)
	{
		if(WorldEntities[i]->GetModelNum() != -1)
		{
			//set origin (bsp models don't have an "origin" key/value pair)
			if(WorldEntities[i]->GetClassName() == "phys_model")
			{
				Q3BSP::BSPModel		*model = (Q3BSP::BSPModel *)bspmap->BSPModels() + WorldEntities[i]->GetModelNum();
				Q3BSP::Face			*face = (Q3BSP::Face *)bspmap->faces() + model->m_FirstFace;
				Q3BSP::BSPVertex	*vert;

				Vector3f			origin = Vector3f(0, 0, 0);
				
				int					k, j, num_verts = 0;

				for(k = 0; k < model->m_NumFaces; k++, face++)
					for(j = 0, vert = (Q3BSP::BSPVertex *)bspmap->BSPVertices() + face->m_FirstVertex; j < face->m_NumVertices; j++, vert++)
					{
						origin += vert->m_Position;
						num_verts++;
					}

				origin /= (float)num_verts;

				WorldEntities[i]->SetCenter(origin);
			}

			//model index
			model_entities[WorldEntities[i]->GetModelNum()] = WorldEntities[i];
		}

		WorldEntities[i]->Init();

//		BaselineEntities[i]->Init();
//		*(BaselineEntities[i]) = *(WorldEntities[i]);
	}
}

/*
===============
G_FreeEntities
===============
*/
void G_FreeEntities(void)
{
	int		i;

	//FIXME!!!
//	R_FreeCachedModels();
	for(i = 0; i < numWorldEntities; i++)
	{
		if(WorldEntities[i])
		{
			delete WorldEntities[i];
			WorldEntities[i] = NULL;
		}
/*
		if(BaselineEntities[i])
		{
			delete BaselineEntities[i];
			BaselineEntities[i] = NULL;
		}
*/	}

	numWorldEntities = 0;
}

/*
================
G_EntityByModel
================
*/
Entity *G_EntityByModel(int model_num)
{
	if(model_entities.find(model_num) != model_entities.end())
		return model_entities[model_num];

	return NULL;
}

/*
=====================
CACHED MODELS/SOUNDS
=====================
*/

static std::map < std::string, RenderableMesh* >	cached_models;
static std::map < std::string, Sound* >				cached_sounds;

/*
==============
R_CachedModel
==============
*/
RenderableMesh *R_CachedModel(std::string name, Renderer &renderer, Propertytable *model_props)
{
	Texture2D		*diffuse_tex, *bump_tex;
	RenderableMesh	*mesh;
	Localfile		lf;

	if(cached_models.find(name) != cached_models.end())
		mesh = cached_models[name];
	else
	{
		mesh = new RenderableMesh("entity_mesh", renderer);

		lf.open(name,"rt");
		if(lf.streamIsOk())
		{
			Memmesh memmesh("tempmesh");
			loadOBJ(lf,memmesh,1,true);

			calculateTangentspace(memmesh,0);
			mesh->transferFrom(memmesh);

			mesh->addRef();
		}
		else
		{
			calculateCube(*mesh, 30, 30, 30, false);
			return mesh;
		}
	}

	lf.close();

	if(model_props)
	{
		string		skin;

		model_props->addRef();

		//strip extension
		skin = name.substr(0, name.find_first_of('.'));

		lf.open(skin + ".jpg", "rb");
		if(!lf.streamIsOk())
		{
			lf.open(skin + ".tga", "rb");
			if(!lf.streamIsOk())
				return mesh;
		}
		diffuse_tex = C_LoadImage(lf, ion::video::Pixelformat_RGB888);
		lf.close();
/*
		lf.open("nm.jpg","rb");
		bump_tex = C_LoadImage(lf, ion::video::Pixelformat_RGB888);
		lf.close();
*/
//		model_props->addColorRGB("diffuseColor",1,1,1);
		model_props->add2DTexture("diffuseTexture",diffuse_tex);
//		model_props->add2DTexture("normalMap",bump_tex);
	}
//	pNode->shader(0,pDot3Shader);

	cached_models[name] = mesh;
	return mesh;
}

/*
===================
R_FreeCachedModels
===================
*/
void R_FreeCachedModels(void)
{
	std::map < std::string, RenderableMesh* >::iterator	i;

	for(i = cached_models.begin(); i != cached_models.end(); i++)
		delete (*i).second;

	cached_models.clear();
}

/*
==============
R_CachedSound
==============
*/
Sound *R_CachedSound(std::string name, Audioserver &server, bool is_3D)
{
	Sound		*sound = NULL;
	Localfile	lf;

	if(cached_sounds.find(name) != cached_sounds.end())
		return cached_sounds[name];

	lf.open(name,"rb");
	if(lf.streamIsOk())
		sound = server.createSound(lf, Sound::NoLoop, false, is_3D);

	cached_sounds[name] = sound;
	return sound;
}

/*
===================
R_FreeCachedSounds
===================
*/
void R_FreeCachedSounds(void)
{
	std::map < std::string, Sound* >::iterator	i;

	for(i = cached_sounds.begin(); i != cached_sounds.end(); i++)
		delete (*i).second;

	cached_sounds.clear();
}

//from ION test9
Simplespace			*skyboxspace;
RenderableMeshNode	*pSkyboxNode = NULL;
RenderableMesh		*pSkybox = NULL;
Texture2D			*pSkyboxTex[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
Propertytable		skyboxMats[6];

Simplespace *R_LoadSkybox(string name, Renderer &renderer)
{
	R_FreeSkybox();

	skyboxspace = ::new Simplespace();

	pSkybox=new RenderableMesh("skybox",renderer);
	calculateSkybox(*pSkybox,10,10,10);
	pSkyboxNode=new RenderableMeshNode("skybox_node",*pSkybox);

	skyboxspace->renderablelist().addRenderable(*pSkyboxNode);

	{
		ion::base::Localfile lf;
		const char *filenames[6]={
			"_dn.tga",
			"_up.tga",
			"_bk.tga",
			"_ft.tga",
			"_lf.tga",
			"_rt.tga"
		};

		for (int texnr=0;texnr<6;++texnr) {
			lf.open(name+filenames[texnr],"rb");
			if(lf.streamIsOk())
				pSkyboxTex[texnr]=C_LoadImage(lf,ion::video::Pixelformat_RGB888);
			else
			{
				lf.close();
				R_FreeSkybox();
				return NULL;
			}
			lf.close();

			skyboxMats[texnr].addRef();
			skyboxMats[texnr].addColorRGBA("ambientColor",1,1,1,1);
			skyboxMats[texnr].addColorRGB("diffuseColor",1,1,1);
			skyboxMats[texnr].add2DTexture("diffuseTexture",pSkyboxTex[texnr]);
			pSkyboxNode->materialProperties(texnr,&(skyboxMats[texnr]));
		}
	}

	return skyboxspace;
}

void R_FreeSkybox(void)
{
	for (int i=0;i<6;++i)
		if(pSkyboxTex[i])
		{
			delete pSkyboxTex[i];
			pSkyboxTex[i] = NULL;
		}
	
	if(skyboxspace)
	{
		::delete skyboxspace;
		skyboxspace = NULL;
	}

//	if(pSkyboxNode)
//		delete pSkyboxNode;
}