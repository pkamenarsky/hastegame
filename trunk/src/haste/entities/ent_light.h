#ifndef _ENT_LIGHT_H_
#define _ENT_LIGHT_H_

#include "../EntLib/entity.h"
#include "../common.h"

#include "ent_player.h"

/*
=============
Light Class
=============
*/
class EntLight : public Entity
{

public:
	EntLight(EngineExport *EE);
	EntLight(void); //TODO!!!
	~EntLight(void);

	void Init(void);

	void Think(void);

	void SetProperty(String &key, String &value);
	void GetProperty(String &key, String &value);

protected:
	Light	*light;
	bool	casts_shadows;
	bool	ignorelight;
};

EXPORT_ENTITY_H(EntLight)

#endif
