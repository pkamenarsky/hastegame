#ifndef _ENT_TRIGGER_EXIT_H_
#define _ENT_TRIGGER_EXIT_H_

#include "../EntLib/entity.h"
#include "../common.h"

/*
=============
TriggerExit Class
=============
*/
class EntTriggerExit : public Entity
{

public:
	EntTriggerExit(EngineExport *EE);
	EntTriggerExit(void); //TODO!!!
	~EntTriggerExit(void);

	void Init(void);

	void SetProperty(String &key, String &value);
	void GetProperty(String &key, String &value);

	void Touch(Entity &other);

protected:
	String	map;
};

EXPORT_ENTITY_H(EntTriggerExit)

#endif
