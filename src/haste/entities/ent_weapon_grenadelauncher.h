#ifndef _ENT_WEAPON_GRENADELAUNCHER_H_
#define _ENT_WEAPON_GRENADELAUNCHER_H_

#include "../EntLib/entity.h"
#include "../common.h"

#include "ent_weapon_grenadelauncher.h"

/*
=============
EntWeaponGrenadelauncher Class
=============
*/
class EntWeaponGrenadelauncher : public Entity
{

public:
	EntWeaponGrenadelauncher(EngineExport *EE);
	EntWeaponGrenadelauncher(void); //TODO!!!
	~EntWeaponGrenadelauncher(void);

	void Init(void);

	void Think(void);
	void Touch(Entity &other);

protected:
	int		test;
	float	y;

	float	time, float_time;

	Vector3f	view_velocity;
};

EXPORT_ENTITY_H(EntWeaponGrenadelauncher)

#endif
