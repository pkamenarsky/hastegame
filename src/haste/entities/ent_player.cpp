#include <stdio.h>
#include <conio.h>

#include "ent_player.h"

static Player	*g_player;

static void cmd_move(void)
{
	g_player->cmd_Move();
}

void Player::cmd_Move(void)
{
	char		cmd_s[512], *cmd = cmd_s; 
	int			on, ec = command;

	strcpy(cmd, EE.GetArg(0).cstr());

	//hack
/*
	if(!strcmp(cmd, "coords"))
	{
		if(NumArgs() == 1)
			cprintf("player coords: <%f %f %f>\n", entity.origin[0], entity.origin[1], entity.origin[2]);
		else
		{
			char	temp[256];

			sprintf(temp, "%f %f %f", GetArg(1), GetArg(2), GetArg(3));
			SetProperty("origin", temp);
		}
		return;
	}
*/
	if(!strcmp(cmd, "noclip"))
	{
		noclip = !noclip;
		return;
	}

	if(!strcmp(cmd, "testbox"))
	{
		PH_AddTestBox(origin + Vector3f(0, 150, 0));
		return;
	}

	if(cmd[0] == '+') on = true;
	else if(cmd[0] == '-') on = false;
	cmd++;

	if(!strcmp(cmd, "forward"))
		command = on ? ec | CMD_FORWARD : ec & (~CMD_FORWARD);
	else if(!strcmp(cmd, "back"))
		command = on ? ec | CMD_BACK : ec & (~CMD_BACK);
	else if(!strcmp(cmd, "moveright"))
		command = on ? ec | CMD_MOVE_RIGHT : ec & (~CMD_MOVE_RIGHT);
	else if(!strcmp(cmd, "moveleft"))
		command = on ? ec | CMD_MOVE_LEFT : ec & (~CMD_MOVE_LEFT);
	else if(!strcmp(cmd, "jump"))
		command = on ? ec | CMD_JUMP : ec & (~CMD_JUMP);
	else if(!strcmp(cmd, "attack"))
		command = on ? ec | CMD_SHOOT : ec & (~CMD_SHOOT);
	else command = 0;

	RecursiveCommand(EE.GetArg(0));
	
//	entity.command = 0;

//	Link();
}

static float	Gravity;

Player::Player(EngineExport *EE) : Entity(EE)
{
	g_player = this;

	ClassName = "player";
	state = 0;
	
	Gravity = 800;//CvarValue_i("sv_gravity");

	noclip = FALSE;
	
	EE->AddCommand("+forward", cmd_move);
	EE->AddCommand("+back", cmd_move);
	EE->AddCommand("+moveright", cmd_move);
	EE->AddCommand("+moveleft", cmd_move);
	EE->AddCommand("+jump", cmd_move);
	EE->AddCommand("+attack", cmd_move);

	EE->AddCommand("+attack", cmd_move);

	EE->AddCommand("coords", cmd_move);

	EE->AddCommand("noclip", cmd_move);
	EE->AddCommand("testbox", cmd_move);
		
	
//	SetAABB(Vector3f(-32, -32, -10), Vector3f(32, 32, 32));
	SetAABB(Vector3f(-10, 0, -10), Vector3f(10, 54, 10));

	SetVelocity(Vector3f(0, 0, 0));
	SetOrigin(Vector3f(0, 0, 0));

	SetAngles(Vector3f(0, 100, 0));

	command = 0;
	groundplane = NULL;

	step_time = 0;
	jump_time = 0;
/*
	LoadSound(String("haste01.wma"));

	if(sound)
		sound->play(1, 0, 0, 0, false, true);
*/
//	EE->C_ExecCommand("s_play haste01.wma");
//	Link();
}	

void Player::Init(void)
{
	std::list<Entity*>		info_player_start_elist;
	Entity					*start;

	EE.GetEntityList(info_player_start_elist, String("classname"), String("info_player_deathmatch"));

	if(info_player_start_elist.empty())
		return;

	start = (Entity *)info_player_start_elist.front();
	origin = start->GetOrigin();
	angles = start->GetAngles();

	ph_box = PH_CreateBox(origin + Vector3f(0, 30, 0), Vector3f(25, 60, 25));
}

Player::Player(void)
{
	Player(NULL);
}

Player::~Player(void)
{
}

#define PH_ACCELERATE		10
#define PH_GRAVITY			800
#define PH_FRICTION			6
#define PH_STOPSPEED		100
//#define PH_SPEED			300
#define PH_GROUNDFRICTION	6
#define PH_JUMPSPEED		270

int PH_SPEED = 300;

void Player::Think(void)
{
	bool		haste = FALSE;

	Entity::Think();

	float	ft = EE.Frametime();
	
	fmove = smove = 0;

//	command = CMD_FORWARD;
	
	fmove += (command & CMD_FORWARD) ? PH_SPEED: 0;
	fmove -= (command & CMD_BACK) ? PH_SPEED: 0;

	smove += (command & CMD_MOVE_RIGHT) ? PH_SPEED: 0;
	smove -= (command & CMD_MOVE_LEFT) ? PH_SPEED: 0;

	step_time -= EE.Frametime();
	jump_time -= EE.Frametime();

	//check inventory
	GetBoolProperty(String("item_haste"), haste);
	if(haste)
		PH_SPEED = 400;
	else
		PH_SPEED = 300;
/*
	if(entity.command & CMD_SHOOT && a)
	{
		if(Entity *tmp = SpawnEntity("ent_rocket"))
		{
			tmp->SetOrigin(origin);
			tmp->SetAngles(angles);

			tmp->Think();
		}
		else cprintf("Player::Think: ent_rocket could not be loaded");

		entity.command &= (~CMD_SHOOT);
		a = 0;
	}
*/	
	PMove();

//	angles[2] = 20;

	ClearCommands();
/*
	static	bool bb = true;
	if(bb)
	{
		bb = false;
		EE.ThrowEvent_i(this, "fovwarp -2016 -340 -1008 -1800 -340 -1008", 0);
	}
*/
	dFloat matrix[16];
	memset(matrix, 0, sizeof(matrix));

	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10] = 1;
	matrix[15] = 1;
	
	matrix[12] = origin[0];
	matrix[13] = origin[1] + 30;
	matrix[14] = origin[2];

	NewtonBodySetMatrix(ph_box, matrix);

	EE.Link(this);
}

void Player::Touch(Entity &other)
{
	//...
}

void Player::Event(String &event, Entity *other)
{
//	logstream() << "PLAYER EVENT: " << event << "\n";

	std::stringstream	sstr(event);
	std::string			token;

	sstr >> token;

	if(token == "push"  && groundplane)
	{
		state |= STATE_BEGIN_PUSH;
		sstr >> target_aim[0] >> target_aim[1] >> target_aim[2];
	}
	
	if(token == "airpush")
	{
		Vector3f	p;
		sstr >> p[0] >> p[1] >> p[2];

		velocity += p;
	}
	if(token == "fovwarp")
	{
		Vector3f	p1, p2;

		sstr >> p1[0] >> p1[1] >> p1[2];
		sstr >> p2[0] >> p2[1] >> p2[2];

		fovwarp_plane.normal((p1 - p2).normalize());
		fovwarp_plane.distance(fovwarp_plane.normal() * p2);

		fovwarp_distance = (p1 - p2).length();
/*
		logstream() << "FOVWARP: "	<< fovwarp_plane.normal()[0] << " "
									<< fovwarp_plane.normal()[1] << " "
									<< fovwarp_plane.normal()[2] << " "
									<< fovwarp_plane.distance() << " "
									<< fovwarp_distance;
*/
		state |= STATE_EFFECT_FOVWARP;
	}
}

#define STOP_EPSILON	0.1

static void PH_ClipVelocity(Vector3f &In, Vector3f &Normal, Vector3f &Out, float overbounce)
{
	float	backoff;
	float	change;
	int		i;
	
	backoff = In * Normal * overbounce;
	
	for(i = 0; i < 3; i++)
	{
		change = Normal[i] * backoff;
		Out[i] = In[i] - change;
		if(Out[i] > -STOP_EPSILON && Out[i] < STOP_EPSILON)
			Out[i] = 0.0f;
	}
}

void Player::PH_Accelerate(Vector3f WishDir, float WishSpeed, float Accel)
{
	int		i;
	float	addspeed, accelspeed, currentspeed;
	
	currentspeed = WishDir * velocity;
	addspeed = WishSpeed - currentspeed;
	
	if(addspeed <= 0) return;
	
	accelspeed = Accel * EE.Frametime() * WishSpeed;
	if(accelspeed > addspeed) accelspeed = addspeed;

	for(i = 0; i < 3; i++)
		velocity[i] += accelspeed * WishDir[i];
}

void Player::PH_Friction(void)
{
	float	drop = 0, speed = velocity.length(), newspeed;

	if(speed < 1) return;

	// apply ground friction
	if (groundplane)
		drop = (speed < PH_STOPSPEED ? PH_STOPSPEED : speed) * PH_GROUNDFRICTION * EE.Frametime();

	newspeed = speed - drop;
	if(newspeed < 0) newspeed = 0;

	newspeed /= speed;

	velocity *= newspeed;
}

#define MAX_CLIP_PLANES		5

void Player::SlideMove(void)
{
	int			bumpcount, numbumps = 4;
	int			numplanes = 0;
	int			i, j;

	float		time_left = EE.Frametime();

	Vector3f	primal_velocity, end, dir, planes[MAX_CLIP_PLANES];

	primal_velocity = velocity;
	
	for(bumpcount = 0; bumpcount < numbumps; bumpcount++)
	{
		end = origin + velocity * time_left;
		EE.trace.TraceBox(origin, end, mins, maxs);

		//do we need this?
		if(EE.trace.GetModelnum() != -1 && EE.trace.GetModelnum() != 0)
			EE.ThrowEvent_i(this, String("touch_model"), EE.trace.GetModelnum());
		
		if(EE.trace.GetFraction() > 0)
		{
			origin = EE.trace.GetCollisionPoint();
			numplanes = 0;
		}
		
		if(EE.trace.GetFraction() == 1.0f) break;
		
		time_left -= time_left * EE.trace.GetFraction();
		
		if(numplanes >= MAX_CLIP_PLANES)
		{
			velocity = Vector3f(0, 0, 0);
			break;
		}
		
		if(!EE.trace.GetCollisionPlane())
			continue;

		planes[numplanes++] = EE.trace.GetCollisionPlane()->normal();
		
		for(i = 0; i < numplanes; i++)
		{
			PH_ClipVelocity(velocity, planes[i], velocity, 1.01f);
			
			for(j = 0; j < numplanes; j++)
				if(j != i)
				{
					if(velocity * planes[j] < 0)
						break;
				}
			
			if(j == numplanes) break;
		}
		
		if(i == numplanes)
		{
			if(numplanes != 2)
			{
				velocity = Vector3f(0, 0, 0);
				break;
			}
		
			dir = planes[0] ^ planes[1];
			velocity = dir * (dir * velocity);
		}
		
		if(velocity * primal_velocity <= 0)
		{
			velocity = Vector3f(0, 0, 0);
			break;
		}
	}

}

#define	STEPSIZE	18

#define	MIN_STEP_NORMAL	0.7		// can't step up onto very steep slopes
#define	MAX_CLIP_PLANES	5

void Player::StepSlideMove(void)
{
	Vector3f	start_o, start_v;
	Vector3f	down_o, down_v;
	Vector3f	up, down;
	float		down_dist, up_dist;

	start_o = origin;
	start_v = velocity;

	SlideMove();

	down_o = origin;
	down_v = velocity;

	up = start_o;
	up[1] += STEPSIZE;

	EE.trace.TraceBox(up, up, mins, maxs);

	if (EE.trace.StartSolid())
		return;		// can't step up

	// try sliding above
	origin = up;
	velocity = start_v;

	SlideMove();

	// push down the final amount
	down = origin;
	down[1] -= STEPSIZE;
	EE.trace.TraceBox(origin, down, mins, maxs);
	if (!EE.trace.StartSolid())
	{
		origin = EE.trace.GetCollisionPoint();
	}

#if 0
	VectorSubtract (pml.origin, up, delta);
	up_dist = DotProduct (delta, start_v);

	VectorSubtract (down_o, start_o, delta);
	down_dist = DotProduct (delta, start_v);
#else
	up = origin;

	// decide which one went farther
    down_dist = (down_o[0] - start_o[0])*(down_o[0] - start_o[0])
        + (down_o[2] - start_o[2])*(down_o[2] - start_o[2]);
    up_dist = (up[0] - start_o[0])*(up[0] - start_o[0])
        + (up[2] - start_o[2])*(up[2] - start_o[2]);
#endif

	if (down_dist > up_dist || (EE.trace.GetCollisionPlane() && EE.trace.GetCollisionPlane()->normal()[1] < MIN_STEP_NORMAL))
	{
		origin = down_o;
		velocity = down_v;
		return;
	}
	//!! Special case
	// if we were walking along a plane, then we need to copy the Z over
	velocity[1] = down_v[1];
}

void Player::CategorizePosition(void)
{
	Vector3f	down;

	down = origin;
	down[1] -= 1;

	EE.trace.TraceBox(origin, down, mins, maxs);

	if(EE.trace.GetModelnum() != -1 && EE.trace.GetModelnum() != 0)
		EE.ThrowEvent_i(this, String("touch_model"), EE.trace.GetModelnum());

	if(EE.trace.GetFraction() < 1.0f && EE.trace.GetCollisionPlane())
	{
		p_groundplane = *EE.trace.GetCollisionPlane();
		groundplane = &p_groundplane;
	}
	else
		groundplane = NULL;
}

void Player::AirMove(void)
{
	int			i;
	Vector3f	WishVel, WishDir;
	Vector3f	forward, right, up;

	forward = Vector3f(0, 0, 1);
	right = Vector3f(1, 0, 0);

	forward = camera->transform().rotation().inverted().rotateVector(forward);
	right = camera->transform().rotation().inverted().rotateVector(right);

	if(!noclip)
	{
		forward[1] = 0;
		right[1] = 0;
	}		


	forward.normalize();
	right.normalize();

	float	fm = (float)fmove, sm = (float)smove;

	//temp
	if(fabs(float(fmove)) >= 0.9f  && fabs(float(smove)) >= 0.9f)
	{
		fm /= 1.414f;
		sm /= 1.414f;
	}
	
	for(i = 0; i < 3; i++)
		WishVel[i] = forward[i] * fm + right[i] * sm;
	
	WishDir = WishVel;
	WishDir.normalize();

	if(noclip)
	{
		velocity = Vector3f(0, 0, 0);
		PH_Accelerate(WishDir, WishVel.length(), 1);
		origin += velocity;
		return;
	}

	if(groundplane)
	{
		velocity[1] = 0;
		PH_Accelerate(WishDir, WishVel.length(), PH_ACCELERATE);

		//play sound
		if(step_time < 0 && velocity.length() > 100)
		{
			step_time = 0.3f;
			EE.C_ExecCommand("s_play basehaste/sound/player/footsteps/step1.wav");
		}

	}
	else
	{
		PH_Accelerate(WishDir, WishVel.length(), 1);
		velocity[1] -= Gravity * EE.Frametime();		//add gravity
	}

	if((command & CMD_JUMP) && groundplane)
	{
		command &= (~CMD_JUMP);
		velocity[1] = PH_JUMPSPEED;

		//play sound
		if(jump_time < 0)
		{
			jump_time = 0.2f;
			EE.C_ExecCommand("s_play basehaste/sound/player/doom/jump1.wav");
		}
	}
	if((state & STATE_BEGIN_PUSH) && groundplane)
	{
		state &= (~STATE_BEGIN_PUSH);
		velocity = target_aim;
	}
	else
		state &= (~STATE_BEGIN_PUSH);

	StepSlideMove();
//	SlideMove();
}

void Player::PMove(void)
{
	CategorizePosition();

	PH_Friction();
	AirMove();

	Effects();
}

#define DEG2RAD(x)	((x) * 3.1415926535f / 180.0f)
#define RAD2DEG(x)	((x) * 180.0f / 3.1415926535f)

#define FOVWARP_MAX	130
#define FOVRESTORETIME	1.0f	//FIXME

void Player::Effects(void)
{
//secret :)
#if 1
	//Lord of the rings fovwwarp effect
	if(state & STATE_EFFECT_FOVWARP)
	{
		float		fov_f;
		float		t, z = 200, a, d = 320, amount;
		Vector3f	f, r, u;

		//fovwarp more in the "target" direction
		AngleVectors(angles, f, r, u);
		amount = -(f * fovwarp_plane.normal());

		if(amount < 0) amount = 0;
		if(amount > 1) amount = 1;

		t = fovwarp_distance - fovwarp_plane.distanceFromPlane(origin);
		a = ((fovwarp_distance + t) / fovwarp_distance - 1) * d;

		//get the warped fov
		fov_f = atan(320.0f / (d - a * amount)) * 2.0f;

		//done?
		if(fovwarp_plane.distanceFromPlane(origin) < 0)
		{
			fovwarp_restoretime = FOVRESTORETIME;

			state &= (~STATE_EFFECT_FOVWARP);
			state |= STATE_EFFECT_FOVRESTORE;

			return;
		}

		//bound checks
		if(fov_f > DEG2RAD(FOVWARP_MAX)) fov_f = DEG2RAD(FOVWARP_MAX);
		if(fov_f < 0) fov_f = DEG2RAD(90);

		//save last fov
		if(fovwarp_plane.distanceFromPlane(origin) > 0)
			fovwarp_lastfov = RAD2DEG(fov_f);

		cvars::setCvarIntValue("fov", (int)RAD2DEG(fov_f));
	}
	//restore fov
	else if(state & STATE_EFFECT_FOVRESTORE)
	{
		fovwarp_restoretime -= EE.Frametime();
		float	fov_f = fovwarp_lastfov - (fovwarp_lastfov - 90) * (FOVRESTORETIME - fovwarp_restoretime);

		if(fov_f < 90)
		{
			fov_f = 90;
			state &= (~STATE_EFFECT_FOVRESTORE);
		}

		cvars::setCvarIntValue("fov", (int)fov_f);
	}
#endif
}

/*
======================================================================
*/

void Player::Die(Entity &killer)
{
	//TODO
}

void Player::SetProperty(String &key, String &value)
{
	Entity::SetProperty(key, value);
}

void Player::GetProperty(String &key, String &value)
{
	Entity::GetProperty(key, value);
/*	
	if(!strcmp(key, "classname"))
		strcpy(value, "player");
*/
}

EXPORT_ENTITY(Player)
