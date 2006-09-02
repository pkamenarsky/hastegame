/***************************************************************************
ENT.H
***************************************************************************/

#ifndef _ENT_H_
#define _ENT_H_

#include "../common.h"

//netvar types
#define NV_RELIABLE		0
#define NV_UNRELIABLE	1

#define NV_BYTE			0
#define NV_SHORT		1
#define NV_LONG			2
#define NV_FLOAT		3
#define NV_DOUBLE		4

#define NV_VEC_FLOAT	5

#define NV_NUM_TYPES	6

//net roles
#define NR_CLIENT		0x00
#define NR_SERVER		0x01

#define NR_NONET		0x04
#define NR_SIMULATE		0x08
#define NR_PLAYER		0x10
#define NR_TEMP			0x20
#define NR_NOT_USED		0x40

#define NR_INVISIBLE	0x20

//events
#define EV_IDENTIFY		0x0
#define EV_THINK		0x1
#define EV_DIE			0x2
#define EV_TOUCH		0x3

typedef struct
{
	int		net_role;
	
	void	(*Trace)(float *start, float *end, float *mins, float *maxs);
	
//	cvar_t	*(*GetCvar)(char *name, char *default_value, int flags);
	void	(*AddCommand)(char *name, void *(Cmd)(void));

	void	(*C_ExecCommand)(char *cmd);
	
	char	*(*CvarValue_s)(char *name);
	int		(*CvarValue_i)(char *name);
	
	void	(*ThrowEvent_s)(int entity_num, char *event, int arg);
	void	(*ThrowEvent_i)(int entity_num, int event, int arg);
	
	void	(*RegisterEvent_s)(int ent_num, char *event, void (*func)(int));
	void	(*RegisterEvent_i)(int ent_num, int event, void (*func)(int));

	void	*(*SpawnEntity)(char *name, int net_role);
	void	(*DestroyEntity)(void *ent);
	void	(*GetEntities)(void *list, char *key, char *value, int net_role);
	
	char	*(*GetArg)(int arg_num);
	int		(*NumArgs)(void);
	
	float	*frametime;
	int		num_entity;
}game_import_t;

#endif
