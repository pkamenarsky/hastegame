#ifndef _G_PHYSICS_H_
#define _G_PHYSICS_H_

#define MAX_ENTITY_BODIES	1024

void PH_Init(void);
void PH_Shut(void);

void PH_InitMap(Q3BSP *bsp);
void PH_DestroyMap(void);

void PH_Frame(void);

void PH_AddTestBox(Vector3f pos);
NewtonBody *PH_CreateBox(Vector3f pos, Vector3f dim);

void PH_AddBspModel(Entity *ent);
void PH_DestroyBspModels(void);

#endif