#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "common.h"

extern bool			done;

extern Renderer		*m_pRenderer;
extern Videodevice	*m_pVideodevice;
extern Q3BSP		*bspmap;
extern Camera		*m_pCamera;
extern Entity		*player;

extern App			*pApp;


extern Entity		*WorldEntities[MAX_WORLD_ENTITIES];
extern Entity		*BaselineEntities[MAX_WORLD_ENTITIES];
extern int			numWorldEntities;

extern std::map <int, Entity*>	model_entities;

#endif