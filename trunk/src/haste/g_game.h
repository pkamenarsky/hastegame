#ifndef _G_GAME_H_
#define _G_GAME_H_

#include "common.h"

void	G_Frame(void);
void	G_Init(Videodevice &videodevice, Audioserver &as);

void	G_DestroyEntity(Entity *ent);

void	G_FreeEntities(void);
void	G_EndMap(void);
void	G_Intermission(String name, bool loading);

float	G_Frametime(void);
float	G_LastFrametime(void);

void	R_DrawSkybox(void);
void	G_GlobalFrame(Vector3f angles);
bool	G_MapLoaded(void);

int		G_Demostate(void);

void	cmd_stop(void);


#endif