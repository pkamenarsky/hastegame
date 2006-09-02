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

console.c
==============================================================================
*/

#include "common.h"
#include "keys.h"

#include "../video/video_all.hh"

#define CONSOLE_X	78
#define CONSOLE_Y	100

#define C_CURSOR_CHAR	'_'
static char	C_CURSOR = C_CURSOR_CHAR;

static int	c_old_time = 0;

static char	cmd_buffer[1024];
static int	cmd_buffer_pos = 0;

static char	console_buffer[CONSOLE_Y][CONSOLE_X];

static int	console_line = 89;
static float	console_pic_y = -0.5;

int			console_image = -1;

int			console_activated = 0;
int			wm_char_ignore = 0;

static int	x = 0;

//ION stuff
static Videodevice	*vd;

static Texture2D	*pic;
static Draw2DCache	*dcache;
static Font			*font;

//toggle_console
void cmd_toggle_console(void)
{
	console_activated = !console_activated;

	if(console_activated)
	{
//		C_Print("]bash on.\n");
		memset(console_buffer[CONSOLE_Y - 1], 0, CONSOLE_X);
		x = 0;
		C_Print("#%c", C_CURSOR);
	}
	else
	{
//		C_Print("bash off.\n");
//		console_buffer[CONSOLE_Y - 1][0] = '\0';
	}
}

void C_ScrollUp(void)
{
	int		i;

	for(i = 1; i < CONSOLE_Y; i++)
		memcpy(console_buffer[i - 1], console_buffer[i], CONSOLE_X);

	memset(console_buffer[CONSOLE_Y - 1], 0, CONSOLE_X);
}


void C_ScrollDown(void)
{
	int		i;

	for(i = CONSOLE_Y - 1; i >= 1; i--)
		memcpy(console_buffer[i], console_buffer[i - 1], CONSOLE_X);

	memset(console_buffer[0], 0, CONSOLE_X);
}

char C_DelChar(void)
{
	char	c;

	if(x > 0)
	{
		c = console_buffer[CONSOLE_Y - 1][--x];
		console_buffer[CONSOLE_Y - 1][x] = '\0';
	}
	else
	{
		C_ScrollDown();

		c = 0;
		x = CONSOLE_X - 2;
	}

	return c;
}

void C_PrintString(char *str)
{
	while(str[0])
	{
		if(str[0] == '\n' || x == CONSOLE_X - 2)
		{
			console_buffer[CONSOLE_Y - 1][x] = '\0';

			C_ScrollUp();
			x = 0;
		}
		else
			console_buffer[CONSOLE_Y - 1][x++] = str[0];

		str++;
	}
}

void C_Print(char *fmt, ...)
{
	va_list vl;
	char	str[1021];
	
	va_start(vl, fmt);
	
	vsprintf(str, fmt, vl);
	C_PrintString(str);
	
	va_end(vl);
}

void C_InitConsole(Videodevice &videodevice, const String &pic_name, const String &font_name)
{
	memset(console_buffer, 0, CONSOLE_X * CONSOLE_Y);

	cvars::addCmd("toggle_console", cmd_toggle_console);
	cvars::addCmd("tconsole", cmd_toggle_console);
	cvars::addCmd("tc", cmd_toggle_console);

	//ION
	vd = &videodevice;

	dcache = new Draw2DCache(videodevice, 1000);
	if(!dcache->isValid())
		return;

	Localfile	lf;
	lf.open(pic_name, "rb");

	pic = load2DTex(videodevice, pic_name, lf, video::Pixelformat_RGB888);
	
	lf.close();

	ion::video::Font::initFontsystem();

	font = new Font(*dcache);

	lf.open(font_name,"rb");
	font->load(lf,20);

	lf.close();
#ifdef _WIN32
//	AllocConsole();
#endif
	cprintf("--- C_InitConsole ---\n");
}

void C_FreeConsole(void)
{
	delete dcache;
	delete pic;
	delete font;
}

bool C_ForwardKey(int key)
{
//	if(key == 164) return;
	if(!console_activated)
		return false;

	if(key == '+')
	{
		if(console_line > 0)
			console_line--;

		return true;
	}

	if(key == '#')
	{
		if(console_line < 89)
			console_line++;

		return true;
	}
#if 0	//autocompletion
	if(key == K_TAB)
	{
		char	str[256];
		int		one_m;

		C_DelChar();

		strncpy(str, cmd_buffer, cmd_buffer_pos)[cmd_buffer_pos] = '\0';
		if((one_m = PrintMatching(str)) == -1)
		{
			C_Print("%c", C_CURSOR);
			return true;
		}

		C_Print("\n]%s%c", str, C_CURSOR);

		if(one_m)
		{
			strcpy(cmd_buffer, str);
			cmd_buffer_pos = strlen(str);
		}
	}
#endif
	if(key == K_BACKSPACE)
	{
		if(cmd_buffer_pos == 0)
			return true;

		C_DelChar();
		C_DelChar();

		C_Print("%c", C_CURSOR);

		cmd_buffer[--cmd_buffer_pos] = '\0';
	}
	else if(key != 13)	//ENTER
	{
		C_DelChar();
		C_Print("%c%c", key, C_CURSOR);

		cmd_buffer[cmd_buffer_pos++] = key;
	}
	else
	{
		C_DelChar();
		C_Print("\n");

		cmd_buffer[cmd_buffer_pos] = '\0';
		cmd_buffer_pos = 0;
		
		cvars::execString(cmd_buffer);
		
		cmd_buffer[0] = '\0';
	
		if(console_activated)
			C_Print("#%c", C_CURSOR);
	}

	return true;
}

static void C_Draw(int start_line, int num_lines, ion::video::Font *font)
{
	char	str[CONSOLE_X + 12];
	int		i, y = 0;
/*
	if(console_image != -1)
		re->R_DrawImage(console_image, 0, console_pic_y, 640, console_pic_y + 240);
*/
	if(!console_activated)
	{
		if(console_pic_y > -0.5)
			console_pic_y -= 0.01f;
		
		return;
	}
	else
	{
		if(console_pic_y < -0.01)
		{
			console_pic_y += 0.01f;
			return;
		}
	}

	for(i = start_line, y = 1; i < start_line + num_lines; i++, y += 1)
	{
		strncpy(str, console_buffer[i], CONSOLE_X - 1);

//		re->DrawString(str, 1, y);
		font->drawText(str, 10, y * 20);
	}
}

void C_DrawConsole(void)
{
//	int		temp = Clock();

#if 0
	// ;)
	if(temp - c_old_time > 500)
	{
		c_old_time = temp;
		
		//flip cursor char
		C_CURSOR = (C_CURSOR == ' ') ? C_CURSOR_CHAR : ' ';
	}
#endif

	//ION
	math::FloatRectangle src(0, 0.5, 1, 0.5);

//	dcache->begin();
//	if(console_activated)
	{
//		vd->blendfunc(ion::video::BlendFunc_Src_Alpha,ion::video::BlendFunc_One_Minus_Src_Alpha);

		vd->fixedLighting(false);
		vd->zbuffer(false);
		vd->blending(false);
		dcache->begin();
		dcache->addImage(*pic, &src, math::FloatRectangle(0, console_pic_y, 1.01f, console_pic_y + 0.5f), Draw2DCache::AddImageMode_ScreenNormalizedCoordinates);
		dcache->end();

		vd->blendfunc(ion::video::BlendFunc_Src_Alpha,ion::video::BlendFunc_One_Minus_Src_Alpha);
		vd->blending(true);
		dcache->begin();
		C_Draw(console_line, 11, font);
		dcache->end();
	}
//	dcache->end();
	vd->blendfunc(ion::video::BlendFunc_One,ion::video::BlendFunc_Zero);
	vd->blending(false);
	vd->zbuffer(true);
}

void C_DrawText(string str, float x, float y, float w, float h, Color *color, Draw2DCache::AddImageMode mode)
{
	math::FloatRectangle	rect(x, y, w, h);
	if(!color)
		color = new Color(1, 1, 1, 1);

	vd->fixedLighting(false);
	vd->zbuffer(false);

	vd->blendfunc(ion::video::BlendFunc_Src_Alpha,ion::video::BlendFunc_One_Minus_Src_Alpha);
	vd->blending(true);

	dcache->begin();
	font->drawText(str, rect, *color, mode);
	dcache->end();

	vd->blending(false);
}

void C_DrawImage(Texture2D	*pic, math::FloatRectangle *src, math::FloatRectangle *dst, Draw2DCache::AddImageMode mode)
{
	vd->fixedLighting(false);
	vd->zbuffer(false);

//	vd->blendfunc(ion::video::BlendFunc_Src_Alpha,ion::video::BlendFunc_One_Minus_Src_Alpha);
	vd->blendfunc(ion::video::BlendFunc_One,ion::video::BlendFunc_One);
	vd->blending(true);

	dcache->begin();
//	dcache->addImage(*pic, src, *dst, Draw2DCache::AddImageMode_ScreenNormalizedCoordinates);
	dcache->addImage(*pic, src, *dst, mode);
	dcache->end();

//	vd->blending(false);
}

Texture2D *C_LoadImage(LocalStreamable &stream, Pixelformat pixelformat)
{
	return load2DTex(*vd, "generic_image" , stream, pixelformat);
}


bool C_IsActive(void)
{
	return console_activated;
}