#ifndef _ENT_HASTE_H_
#define _ENT_HASTE_H_

#include "../EntLib/entity.h"
#include "../common.h"

#include "ent_haste.h"

/*
=============
Haste Class
=============
*/
class EntHaste : public Entity
{

public:
	EntHaste(EngineExport *EE);
	EntHaste(void); //TODO!!!
	~EntHaste(void);

	void Init(void);

	void Think(void);
	void Touch(Entity &other);

protected:
	float	float_time, y;
};

EXPORT_ENTITY_H(EntHaste)

#endif
