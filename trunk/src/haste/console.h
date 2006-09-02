/*
==============================================================================
Lucifer Engine source code
Copyright (c) 2001 - 2002 by Morgoth

******************************************************************************
IMPORTANT:

This engine is under an absolutely secret devlopment. If you have a copy
of its source (e.g. you can read this), you are strongly encouraged to
destroy it as fast as you can. In case that you don't, you are breaking
applicable international laws, and your life will most likely be in
great danger.
******************************************************************************

console.h
==============================================================================
*/

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "common.h"

void C_InitConsole(Videodevice &videodevice, const String &pic_name, const String &font_name);
void C_FreeConsole(void);

bool C_ForwardKey(int key);

void C_Print(char *fmt, ...);
void C_PrintString(char *str);
void C_DrawConsole(void);

void C_DrawText(string str, float x, float y, float w = 0, float h = 0.05, Color *color = NULL, Draw2DCache::AddImageMode mode = Draw2DCache::AddImageMode_ScreenNormalizedCoordinates);
void C_DrawImage(Texture2D	*pic, math::FloatRectangle *src, math::FloatRectangle *dst, Draw2DCache::AddImageMode mode = Draw2DCache::AddImageMode_ScreenNormalizedCoordinates);

Texture2D *C_LoadImage(LocalStreamable &stream, Pixelformat pixelformat);

bool C_IsActive(void);

void cmd_toggle_console(void);

#endif
