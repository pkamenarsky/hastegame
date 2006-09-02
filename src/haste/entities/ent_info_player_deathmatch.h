#ifndef _ENT_INFO_PLAYER_DEATHMATCH_H_
#define _ENT_INFO_PLAYER_DEATHMATCH_H_

#include "../EntLib/entity.h"
#include "../common.h"

/*
=============
IPD Class
=============
*/
class EntInfoPlayerDeathmatch : public Entity
{

public:
	EntInfoPlayerDeathmatch(EngineExport *EE);
	EntInfoPlayerDeathmatch(void); //TODO!!!
	~EntInfoPlayerDeathmatch(void);

	void Init(void);
};

EXPORT_ENTITY_H(EntInfoPlayerDeathmatch)

#endif
