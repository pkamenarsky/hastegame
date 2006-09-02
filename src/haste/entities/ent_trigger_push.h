#ifndef _ENT_TRIGGER_PUSH_H_
#define _ENT_TRIGGER_PUSH_H_

#include "../EntLib/entity.h"
#include "../common.h"

/*
=============
TriggerPush Class
=============
*/
class EntTriggerPush : public Entity
{

public:
	EntTriggerPush(EngineExport *EE);
	EntTriggerPush(void); //TODO!!!
	~EntTriggerPush(void);

	void Init(void);

	void SetProperty(String &key, String &value);
	void GetProperty(String &key, String &value);

	void Touch(Entity &other);

protected:
	String	target_string;
	Entity	*target;
};

EXPORT_ENTITY_H(EntTriggerPush)

#endif
