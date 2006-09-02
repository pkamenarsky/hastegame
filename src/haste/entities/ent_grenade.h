#ifndef _ENT_GRENADE_H_
#define _ENT_GRENADE_H_

#include "../EntLib/entity.h"
#include "../common.h"

/*
=============
Grenade Class
=============
*/
class EntGrenade : public Entity
{

public:
	EntGrenade(EngineExport *EE);
	EntGrenade(void); //TODO!!!
	~EntGrenade(void);

	void Init(void);
	void Think(void);

	void SetProperty(String &key, String &value);
	void GetProperty(String &key, String &value);

protected:
	bool	exploded;
	float	time;

	Entity	*light;

	Sound	*bounce[2], *explode;
	Texture2D	*smoke;

	Vector3f	last_smoke_trail;
};

EXPORT_ENTITY_H(EntGrenade)

#endif
