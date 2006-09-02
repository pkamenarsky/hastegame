#ifndef _G_GUI_H_
#define _G_GUI_H_

#include "common.h"

/*
==========================================================================================
*.gui syntax

{
	type button
	name string
	image_up imagename
	image_down imagename
	location "x1 y1 x2 y2"
	text string
	actiontype callmenu/command
	action string
	[deactivate 0/1]
}

...

{
	type menu
	name string
	background image/moviename
	location "x1 y1 x2 y2"
	element name
	element name
	...
}

==========================================================================================
*/

class UI_Base
{
public:
	UI_Base();
	virtual ~UI_Base();

	virtual void SetKeyValue(string key, string value);
	virtual void Draw(float x, float y);
	virtual void Reset(void);

	virtual void Mouse(float startx, float starty, float x, float y, bool &pressed);

	string GetName(void);

protected:
	string		type;
	string		name;
	string		parent;

	ion::math::FloatRectangle	rect;
};

class UI_Button : public UI_Base
{
public:
	UI_Button();
	~UI_Button();

	void SetKeyValue(string key, string value);
	void Draw(float x, float y);
	void Mouse(float startx, float starty, float x, float y, bool &pressed);
	void Reset(void);

protected:
	string		image_up, image_down, cinematic;
	string		text;
	string		actiontype;
	string		action;

	float		fps, time;
	int			frame;
	bool		end, loop;

	bool		deactivate;

	bool		state;

	//image
	Texture2D	*tex_up, *tex_down;
	Color		color;
};

class UI_Menu : public UI_Base
{
public:
	UI_Menu();
	~UI_Menu();

	void SetKeyValue(string key, string value);
	void Draw(float startx, float starty);
	void Mouse(float startx, float starty, float x, float y, bool &pressed);
	void Reset(void);

protected:
	string				background;
	list <string>		elements;

	Texture2D			*image;
};

void UI_Load(Streamable &is);
void UI_Free(void);

UI_Base *UI_GetElemByName(string name);

void UI_Test(void);

void UI_ActivateMenu(String name, bool exclusive);
void UI_DeactivateMenu(String name);

void UI_Draw(float relx, float rely, bool pressed);
void UI_Init(void);

#endif