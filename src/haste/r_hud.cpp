#include "common.h"

//gametimer
#define GT_NODRAW	0
#define GT_RUNNING	1
#define GT_STOPPED	2

static Timer	gametimer;
static int		gametimer_status;

static int		lastsecs;

Timer			fpstimer;

//global map timer stuff
static void cmd_starttimer(void)
{
	gametimer.update();
	gametimer.adjustPivotTime();
	gametimer_status = GT_RUNNING;

	lastsecs = 0;
}

static void cmd_stoptimer(void)
{
	if(gametimer_status == GT_STOPPED)
		gametimer_status = GT_NODRAW;
	else if(gametimer_status == GT_RUNNING)
		gametimer_status = GT_STOPPED;
}


static float			r_frame = 0, r_frametime;

/*
==========
UpdateFPS

handles fps counting as well as limiting (max_fps cvar)
==========
*/
float UpdateFPSCounter(void)
{
	static int		old_time = 0;
	static int		time;
	float			fps;

	static float	s_fps = 0;

	char			c_fps[256];

	bool			draw_fps = TRUE;
	bool			paused;

		//limit fps
//	if(max_fps->v_int)
//		while((time = Clock()) - old_time < 1000.0f / (float)max_fps->v_int);
//	else
//		time = Clock();

//	if(fpstimer.frequency() < 0.000000001f || fpstimer.difference() / fpstimer.frequency() < 0.00000001f)
//		return 0.0f;

	fpstimer.update();
	r_frametime += (float)fpstimer.difference() / fpstimer.frequency();
//	r_frametime = (float)fpstimer.currTimeRel() / fpstimer.frequency();
	fps = r_frame / r_frametime;

	if(!draw_fps)
		return 0.0f;

	r_frame++;

	if(r_frametime > 1.0f)
	{
//		fpstimer.adjustPivotTime();
		s_fps = r_frame / r_frametime;
		r_frametime = 0;
		r_frame = 0;


	}

	sprintf(c_fps, "%d fps", (int)s_fps);
	C_DrawText(string(c_fps), 0.8, 0.042);

	return s_fps;
}

/*
===============
InitFPSCounter
===============
*/
void InitFPSCounter(void)
{
	r_frame = 0;
	r_frametime = 0;
	fpstimer.update();
}

/*
=========
HUD_Init
=========
*/
void HUD_Init(void)
{
	cvars::addCmd("starttimer", cmd_starttimer);
	cvars::addCmd("stoptimer", cmd_stoptimer);

	gametimer_status = GT_NODRAW;

	InitFPSCounter();
}

/*
=========
HUD_Shut
=========
*/
void HUD_Shut(void)
{
}

/*
=========
HUD_Draw
=========
*/
//FIXME
void HUD_Draw(void)
{
	int				secs, msecs;
	stringstream	sstr;
	bool			paused;

	String			ammo;
	UI_Base			*hud_ammo;

	hud_ammo = UI_GetElemByName("hud_ammo");
	player->GetProperty(String("rockets"), ammo);
	hud_ammo->SetKeyValue("text", ammo.STLstring());

	cvars::getCvarBoolValue("paused", paused);

	UpdateFPSCounter();

	if(gametimer_status == GT_RUNNING && !paused)
		gametimer.update();

//	if(paused)
//		lastsecs = gametimer.currTimeRel() / gametimer.frequency();

	if(gametimer_status != GT_NODRAW)
	{
		secs = gametimer.currTimeRel() / gametimer.frequency();

		msecs = 100.0f * (gametimer.currTimeRel() - (secs * gametimer.frequency())) / gametimer.frequency();
		if(msecs > 100) msecs = 100;

		sstr << "Time: " << secs << ":";

		sstr.fill('0');
		sstr.width(2);
		sstr << msecs;

		C_DrawText(sstr.str(), 0.42, 0.042);
	}
}

/*
=============
HUD_Activate
=============
*/
void HUD_Activate(void)
{
	UI_ActivateMenu("hud_menu", FALSE);
}

/*
===============
HUD_Deactivate
===============
*/
void HUD_Deactivate(void)
{
	UI_DeactivateMenu("hud_menu");
}