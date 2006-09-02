#ifndef _ENT_PHYS_MODEL_H_
#define _ENT_PHYS_MODEL_H_

#include "../EntLib/entity.h"
#include "../common.h"

/*
=============
Rocket Class
=============
*/
class EntPhysModel : public Entity
{

public:
	EntPhysModel(EngineExport *EE);
	EntPhysModel(void); //TODO!!!
	~EntPhysModel(void);

	void Init(void);
	void Think(void);
	void Event(String &event, Entity *other);
	void Touch(Entity &other);

	void SetProperty(String &key, String &value);
	void GetProperty(String &key, String &value);

protected:
	Vector3f	add_velocity, rotation;
	String		model;
	bool		physics;
};

EXPORT_ENTITY_H(EntPhysModel)

#endif
