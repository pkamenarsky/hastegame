#ifndef _ENT_FUNC_FOVWARP_H_
#define _ENT_FUNC_FOVWARP_H_

#include "../EntLib/entity.h"
#include "../common.h"

#include "ent_func_fovwarp.h"

/*
=============
EntFuncFovwarp Class
=============
*/
class EntFuncFovwarp : public Entity
{

public:
	EntFuncFovwarp(EngineExport *EE);
	EntFuncFovwarp(void); //TODO!!!
	~EntFuncFovwarp(void);

	void Init(void);

	void Think(void);
	void Touch(Entity &other);

	void SetProperty(String &key, String &value);
	void GetProperty(String &key, String &value);

protected:
	String	target_string;
	Entity	*target;

	bool	touched;
};

EXPORT_ENTITY_H(EntFuncFovwarp)

#endif
