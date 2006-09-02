#include "common.h"

static Audioserver		*audioserver;

//our "playlist"
static list <string>	playlist;
static int				loopmode;

static map <string, Sound*>	active_sounds;

static string			active_music;

//enqueue in playlist
void cmd_s_enqueue(void)
{
	if(cvars::numArgs() < 2)
	{
		logstream() << "Usage: s_enqueue track\n";
		return;
	}

	playlist.push_back(cvars::getArg(1));
}

void cmd_s_loopmode(void)
{
	string	s;

	if(cvars::numArgs() < 2)
	{
		logstream() << "Usage: s_loopmode none|track|all\n";
		return;
	}

	loopmode = MUSIC_LOOP_ALL;
	s = cvars::getArg(1);

	if(s == "none")
		loopmode = MUSIC_LOOP_NONE;
	else if(s == "track")
		loopmode = MUSIC_LOOP_TRACK;
}

//play immediately
void cmd_s_play(void)
{
	Sound	*s = NULL;

	if(cvars::numArgs() < 2)
	{
		logstream() << "Usage: s_play track/sound\n";
		return;
	}

	if(audioserver)
		s = R_CachedSound(cvars::getArg(1), *audioserver);
	else
		return;

	if(!s)
		return;

//	active_sounds[cvars::getArg(1)] = s->play(0.2, 0, 0, 0, false, true);
	s->play(0.2, 0, 0, 0, false, true);
	active_sounds[cvars::getArg(1)] = s;
}

void cmd_s_stop(void)
{
	Sound	*s = NULL;

	if(cvars::numArgs() < 2)
	{
		logstream() << "Usage: s_stop track/sound\n";
		return;
	}

	if(active_sounds.find(cvars::getArg(1)) == active_sounds.end())
		return;

	delete active_sounds[cvars::getArg(1)];
	active_sounds.erase(cvars::getArg(1));
}

void S_Init(Audioserver *v_audioserver)
{
	audioserver = v_audioserver;

	cvars::addCmd("s_play", cmd_s_play);
	cvars::addCmd("s_stop", cmd_s_stop);
	cvars::addCmd("s_enqueue", cmd_s_enqueue);
	cvars::addCmd("s_loopmode", cmd_s_loopmode);
}

void S_Stop(string name)
{
	Sound	*s = NULL;

	if(active_sounds.find(name) == active_sounds.end())
		return;

	delete active_sounds[name];
	active_sounds.erase(name);
}


void S_Play(string name)
{
	Sound	*s = NULL;

	if(audioserver)
		s = R_CachedSound(name, *audioserver);
	else
		return;

	if(!s)
		return;

	s->play(0.2, 0, 0, 0, false, true);
	active_sounds[name] = s;
}

void S_Music(string music)
{
	if(active_music != "")
		S_Stop(active_music);

	active_music = music;

	if(music == "")
		return;

	S_Play(music);
}

void S_Update(void)
{
	if(!audioserver)
		return;

	audioserver->listenerAttributes(player->GetOrigin(), player->GetVelocity());
	audioserver->update();
}