#ifndef _ENT_ROCKET_H_
#define _ENT_ROCKET_H_

#include "../EntLib/entity.h"
#include "../common.h"

/*
=============
Rocket Class
=============
*/
class EntRocket : public Entity
{

public:
	EntRocket(EngineExport *EE);
	EntRocket(void); //TODO!!!
	~EntRocket(void);

	void Init(void);
	void Think(void);

	void SetProperty(String &key, String &value);
	void GetProperty(String &key, String &value);

protected:
	float		time;

	bool		exploded;
	Entity		*light;

	Sound		*explode;
	Texture2D	*smoke[10], *boom[8];

	Vector3f	last_smoke_trail;
};

EXPORT_ENTITY_H(EntRocket)

#endif
