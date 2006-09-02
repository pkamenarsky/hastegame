#ifndef _G_AUDIO_H_
#define _G_AUDIO_H_

#include "common.h"

#define MUSIC_LOOP_NONE		0x0
#define MUSIC_LOOP_TRACK	0x1
#define MUSIC_LOOP_ALL		0x2

void S_Init(Audioserver *v_audioserver);
void S_Update(void);
void S_Play(string name);
void S_Stop(string name);
void S_Music(string music);

#endif