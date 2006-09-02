#ifndef _ENT_PLAYER_H_
#define _ENT_PLAYER_H_

#include "../EntLib/entity.h"
#include "../common.h"

//movement commands
#define CMD_FORWARD			0x01
#define CMD_BACK			0x02
#define CMD_MOVE_LEFT		0x04
#define CMD_MOVE_RIGHT		0x08

//action commands
#define CMD_SHOOT			0x10
#define CMD_JUMP			0x20
#define CMD_CROUCH			0x40
#define CMD_ACTIVATE		0x80

//states
#define STATE_BEGIN_PUSH		0x01
#define STATE_EFFECT_FOVWARP	0x40
#define STATE_EFFECT_FOVRESTORE	0x80

/*
=============
Player Class
=============
*/
class Player : public Entity
{

public:
	Player(EngineExport *EE);
	Player(void); //TODO!!!
	~Player(void);

	void Init(void);

	void Think(void);
	void Die(Entity &killer);
	void Touch(Entity &other);
	void Event(String &event, Entity *other = NULL);
	
	void SetProperty(String &key, String &value);
	void GetProperty(String &key, String &value);

	//commands	
	void cmd_Move(void);

	//effects
	void Effects(void);

protected:
	NewtonBody	*ph_box;

	int 	health;
	int		armor;

	int		command;
	int		state;
	
	//physics
	void PH_Accelerate(Vector3f WishDir, float WishSpeed, float Accel);
	void PH_Friction(void);
	void AirMove(void);
	
	//movement
	int			fmove, smove;

	Plane		p_groundplane;
	Plane		*groundplane;

	Vector3f	target_aim;

	bool		noclip;

	float		step_time, jump_time;

	//effects
	Plane		fovwarp_plane;
	float		fovwarp_distance;

	float		fovwarp_lastfov;
	float		fovwarp_restoretime;
	
	void Player::CategorizePosition(void);

	void SlideMove(void);
	void StepSlideMove(void);

	void PMove(void);
};

EXPORT_ENTITY_H(Player)

#endif
