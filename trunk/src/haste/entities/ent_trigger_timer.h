#ifndef _ENT_TRIGGER_TIMER_H_
#define _ENT_TRIGGER_TIMER_H_

#include "../EntLib/entity.h"
#include "../common.h"

/*
=============
TriggerTimer Class
=============
*/
class EntTriggerTimer : public Entity
{

public:
	EntTriggerTimer(EngineExport *EE);
	EntTriggerTimer(void); //TODO!!!
	~EntTriggerTimer(void);

	void Init(void);

	void SetProperty(String &key, String &value);
	void GetProperty(String &key, String &value);

	void Touch(Entity &other);

protected:
	String	action;
	int		state;
};

EXPORT_ENTITY_H(EntTriggerTimer)

#endif
