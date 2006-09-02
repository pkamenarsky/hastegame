#ifndef _ENT_TRIGGER_RESTART_H_
#define _ENT_TRIGGER_RESTART_H_

#include "../EntLib/entity.h"
#include "../common.h"

/*
=============
TriggerRestart Class
=============
*/
class EntTriggerRestart : public Entity
{

public:
	EntTriggerRestart(EngineExport *EE);
	EntTriggerRestart(void); //TODO!!!
	~EntTriggerRestart(void);

	void Init(void);

	void SetProperty(String &key, String &value);
	void GetProperty(String &key, String &value);

	void Touch(Entity &other);

protected:
};

EXPORT_ENTITY_H(EntTriggerRestart)

#endif
