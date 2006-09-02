#ifndef _ENT_WRAP_H_
#define _ENT_WRAP_H_

#ifdef __cplusplus
extern "C" {
#endif

void EW_RegisterEntityWrap(void);

struct entity_c;

struct entity_c *EW_LoadPlayerEntity(char *name, int net_role);

int EW_LoadWorldEntity(char *name, int net_role);
void EW_LoadWorldEntities(int net_role);
void EW_UnloadWorldEntities(int net_role);
void EW_EntityThink(int ent_num, int net_role);

#ifdef __cplusplus
}	//extern "C"

#define ENT_WRAP extern "C"
#else
#define ENT_WRAP
#endif

#endif