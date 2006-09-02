#ifndef _ENT_WEAPON_ROCKETLAUNCHER_H_
#define _ENT_WEAPON_ROCKETLAUNCHER_H_

#include "../EntLib/entity.h"
#include "../common.h"

#include "ent_weapon_rocketlauncher.h"

/*
=============
EntWeaponRocketlauncher Class
=============
*/
class EntWeaponRocketlauncher : public Entity
{

public:
	EntWeaponRocketlauncher(EngineExport *EE);
	EntWeaponRocketlauncher(void); //TODO!!!
	~EntWeaponRocketlauncher(void);

	void Init(void);

	void Think(void);
	void Touch(Entity &other);

protected:
	int		test;
	float	y;

	float	time, float_time;

	Vector3f	view_velocity;
};

EXPORT_ENTITY_H(EntWeaponRocketlauncher)

#endif
