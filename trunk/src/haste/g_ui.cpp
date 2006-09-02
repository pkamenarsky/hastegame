#include "common.h"
#include <iomanip>

static list <UI_Base*>				ui_elements;
typedef list <UI_Base*>::iterator	ui_base_iterator;

static map <string, UI_Base*>		ui_map;

static map <string, bool>			ui_active_menus;
typedef map <string, bool>::iterator	ui_menu_iterator;

static map <string, bool>			ui_menu_exclusiveness;
typedef map <string, bool>::iterator	ui_exclusive_iterator;

static float						ui_cursor_x, ui_cursor_y;
static int							click_state = 0;

static Texture2D					*ui_cursor;

static Timer						timer;

UI_Base *UI_GetElemByName(string name)
{
	if(ui_map.find(name) != ui_map.end())
		return ui_map[name];

	return NULL;
}

bool UI_InsideRect(math::FloatRectangle &rect, float x, float y)
{
	if(	x >= rect.left() && y >= rect.top() &&
		x - rect.left() <= rect.width() && y - rect.top() <= rect.height())
			return TRUE;

	return FALSE;
}

/*
========
UI_Base
========
*/
UI_Base::UI_Base()
{
}

UI_Base::~UI_Base()
{
}

void UI_Base::Reset(void)
{
}

void UI_Base::SetKeyValue(string key, string value)
{
	if(key == "parent")
		parent = value;
	if(key == "type")
		type = value;
	else if(key == "name")
		name = value;	
	else if(key == "location")
	{
		stringstream	sstr(value);
		float			f;

		sstr >> f;
		rect.left(f);

		sstr >> f;
		rect.top(f);

		sstr >> f;
		rect.width(f);

		sstr >> f;
		rect.height(f);
	}
}

string UI_Base::GetName(void)
{
	return name;
}

void UI_Base::Draw(float x, float y)
{
	//nop
}

void UI_Base::Mouse(float startx, float starty, float x, float y, bool &pressed)
{
	//nop
}

/*
==========
UI_Button
==========
*/

UI_Button::UI_Button()
{
	actiontype = "";
	action = "";
	parent = "";
	deactivate = 0;
	cinematic = "";

	text = "";
	end = FALSE;

	tex_up = NULL;
	tex_down = NULL;

	state = 0;

	color.r(0.5);
	color.g(0.5);
	color.b(1);
}

void UI_Button::Reset(void)
{
	end = FALSE;
	frame = 0;

	color.r(0.5);
	color.g(0.5);
	color.b(1);
}

UI_Button::~UI_Button()
{
	if(tex_up) delete tex_up;
	if(tex_down) delete tex_down;
}

void UI_Button::SetKeyValue(string key, string value)
{
	Localfile	lf;

	UI_Base::SetKeyValue(key, value);

	if(key == "image_up")
	{
		image_up = value;

		lf.open(value, "rb");
		if(lf.streamIsOk())
			tex_up = C_LoadImage(lf, Pixelformat_RGB888);

		lf.close();
	}
	else if(key == "image_down")
	{
		image_down = value;

		lf.open(value, "rb");
		if(lf.streamIsOk())
			tex_down = C_LoadImage(lf, Pixelformat_RGB888);

		lf.close();
	}
	else if(key == "cinematic")
	{
		stringstream	sstr(value);

		sstr >> cinematic;
		sstr >> fps;
		sstr >> loop;

		end = FALSE;
		time = 1.0f / fps;
		frame = 0;
	}
	else if(key == "text")
		text = value;
	else if(key == "actiontype")
		actiontype = value;
	else if(key == "action")
		action = value;
	else if(key == "deactivate")
		deactivate = value == "1" ? true : false;
}

void UI_Button::Draw(float x, float y)
{
	if(!end && cinematic != "")
	{
		Localfile	lf;
		Texture2D	*tex;
		stringstream	framename;

		math::FloatRectangle	dst = rect;

		dst.left(rect.left() + x);
		dst.top(rect.top() + y);	

		timer.update();
		time -= (float)timer.difference() / timer.frequency();

		while(time < 0)
		{
			frame++;
			time += 1.0 / fps;
		}

		framename << cinematic << setfill('0') << setw(3) << frame << setw(4) << ".jpg";
//		framename << cinematic << "00" << frame << ".jpg";
		C_Print("%s\n", framename.str().c_str());

		lf.open(framename.str(), "rb");
		if(lf.streamIsOk())
			tex = C_LoadImage(lf, Pixelformat_RGB888);
		else if(loop)
		{
			lf.close();
			frame = 0;
			time = 1.0f / fps;
			return;
		}
		else
		{
			lf.close();
			end = TRUE;
			UI_DeactivateMenu(parent);
			return;
		}

		lf.close();

		C_DrawImage(tex, NULL, &dst);
		delete tex;
		return;
	}
	if(!tex_up || !tex_down)
	{
		C_DrawText(text, x + rect.left(), y + rect.top(), 0, rect.height(), &color);
	}
	else
	{
		math::FloatRectangle	dst = rect;

		dst.left(rect.left() + x);
		dst.top(rect.top() + y);
		
		if(state == 0)
			C_DrawImage(tex_up, NULL, &dst);
		else if(state == 1)
			C_DrawImage(tex_down, NULL, &dst);
	}
}

void UI_Button::Mouse(float startx, float starty, float x, float y, bool &pressed)
{
	if(UI_InsideRect(rect, x - startx, y - starty))
		if(pressed)
			state = 2;
		else
			state = 1;
	else
		state = 0;

	if(state == 1)
	{
		color.r(1);
		color.g(0.2);
		color.b(0.2);
	}
	else
	{
		color.r(0.5);
		color.g(0.5);
		color.b(1);
	}

	if(pressed && UI_InsideRect(rect, x - startx, y - starty))
	{
		if(deactivate)
			UI_DeactivateMenu(parent);

		if(action != "")
			if(actiontype == "command")
			{
				pressed = FALSE;
				cvars::execString(action);
			}
			else if(actiontype == "callmenu")
			{
				pressed = FALSE;
				UI_ActivateMenu(action, TRUE);
			}
	}
}

/*
==========
UI_Menu
==========
*/
UI_Menu::UI_Menu()
{
	image = NULL;
}


UI_Menu::~UI_Menu()
{
	if(image)
		delete image;
}

void UI_Menu::Reset(void)
{
	list <string>::iterator	si;
	UI_Base					*elem;

	for(si = elements.begin(); si != elements.end(); si++)
	{
		elem = UI_GetElemByName(*si);
		if(elem)
			elem->Reset();
	}
}

void UI_Menu::SetKeyValue(string key, string value)
{
	Localfile	lf;

	UI_Base::SetKeyValue(key, value);

	if(key == "background")
	{
		background = value;

		lf.open(value, "rb");
		if(lf.streamIsOk())
			image = C_LoadImage(lf, Pixelformat_RGB888);

		lf.close();
	}
	else if(key == "element")
	{
		UI_Base		*e = UI_GetElemByName(value);
		if(e)
			e->SetKeyValue("parent", name);

		elements.push_front(value);
	}
}

void UI_Menu::Draw(float x, float y)
{
	list <string>::iterator	si;
	UI_Base					*elem;

	if(image)
		C_DrawImage(image, NULL, &rect);

	for(si = elements.begin(); si != elements.end(); si++)
	{
		elem = UI_GetElemByName(*si);
		if(elem)
			elem->Draw(rect.left() + x, rect.top() + y);
	}
}

void UI_Menu::Mouse(float startx, float starty, float x, float y, bool &pressed)
{
	list <string>::iterator	si;
	UI_Base					*elem;

	for(si = elements.begin(); si != elements.end(); si++)
	{
		elem = UI_GetElemByName(*si);
		if(elem)
			elem->Mouse(startx + rect.left(), starty + rect.top(), x, y, pressed);
	}
}

/*
========
UI_Load
========
*/
void UI_Load(Streamable &ui_file)
{
	string		token, key, value;
	string		keys[32], values[32];
	int			num_pairs;

	stringstream	is;

	String		Stoken;

	UI_Base		*ui_base;

	ui_elements.clear();
	ui_map.clear();
	ui_active_menus.clear();
	ui_menu_exclusiveness.clear();

	ui_active_menus.clear();

	ui_file >> Stoken;
	is.str(Stoken.STLstring());

	while(!is.eof())
	{
		is >> token;

		if(is.eof())
			break;

		if(token != "{")
		{
			logstream() << "UI_Load: ERROR: missing '{'\n";
			return;
		}

		num_pairs = 0;
		ui_base = NULL;

		while(!is.eof())
		{
			is >> token;

			if(is.eof())
			{
				logstream() << "UI_Load: ERROR: unexpected end of ui string\n";
				return;
			}

			if(token == "}")
				break;

			if(token[0] == '\"')
				key = token.substr(1, token.length() - 2);
			else
				key = token;

			is >> token;

			if(token[0] == '\"' && token[token.length() -1] != '\"')
			{
				string	temp;
	
				do
				{
					temp.erase();

					is >> temp;

					token += " " + temp;
				}while(temp[temp.length() - 1] != '\"');
			}

			if(token[0] == '\"')
				value = token.substr(1, token.length() - 2);
			else
				value = token;
			
			if(num_pairs < 32)
			{
				keys[num_pairs] = key;
				values[num_pairs++] = value;
			}

			if(key == "type")
				if(value == "menu")
					ui_base = new UI_Menu;
				else if(value == "button")
					ui_base = new UI_Button;
		}

		if(ui_base)
		{
			int		i;

			for(i = 0; i < num_pairs; i++)
				ui_base->SetKeyValue(keys[i], values[i]);

			ui_elements.push_front(ui_base);

			if(ui_base->GetName() != "")
				ui_map[ui_base->GetName()] = ui_base;
		}
	}
}

/*
========
UI_Free
========
*/
void UI_Free(void)
{
	ui_base_iterator	ui;

	if(ui_cursor) delete ui_cursor;

	for(ui = ui_elements.begin(); ui != ui_elements.end(); ui++)
		delete (*ui);

	ui_elements.clear();
	ui_map.clear();
	ui_active_menus.clear();
}

void UI_Test(void)
{
	Localfile	lf;
	lf.open("basehaste/ui/test.gui", "r");

	if(lf.streamIsOk())
		UI_Load(lf);

	if(UI_GetElemByName("asmenu"))
		UI_GetElemByName("asmenu")->Draw(0, 0);

	lf.close();
}

//hack omg
static bool	pm = FALSE;

void UI_ActivateMenu(String name, bool exclusive)
{
	UI_Base		*menu;

	ui_active_menus[name] = TRUE;
	ui_menu_exclusiveness[name] = exclusive;
	
	if(exclusive)
		cvars::setCvarBoolValue("paused", TRUE);

	//hehe hack
	if(name == "asmenu")
//		S_Play("basehaste/music/menu.mp3");
		if(!pm)
		{
			S_Music("basehaste/music/menu.mp3");
			pm = TRUE;
		}

	//hehe hack again :)
	if(name == "credits")
		UI_DeactivateMenu("asmenu");

	if(menu = UI_GetElemByName(name))
		menu->Reset();

	timer.update();
}

void UI_DeactivateMenu(String name)
{
	ui_exclusive_iterator	ei;
	ui_menu_iterator		mi;
	bool					exclusive = FALSE;

	ui_active_menus[name] = FALSE;
	ui_menu_exclusiveness[name] = FALSE;

	for(ei = ui_menu_exclusiveness.begin(); ei != ui_menu_exclusiveness.end(); ei++)
	{
		if((*ei).first != name.STLstring())
			if((*ei).second)
				exclusive = TRUE;
	}

	cvars::setCvarBoolValue("paused", exclusive);

	//hehe hack again :)
	if(name == "credits")
		UI_ActivateMenu("asmenu", TRUE);

	//hehe hack
//	if(name == "asmenu")
//		S_Stop("basehaste/music/menu.mp3");
}

void UI_Draw(float relx, float rely, bool pressed)
{
	ui_exclusive_iterator	ei;
	UI_Base					*menu;
	ui_menu_iterator		mi;
	bool					exclusive = FALSE, click = FALSE;

	if(click_state == 0 && pressed)
	{
		click = TRUE;
		click_state = 1;
	}

	if(click_state == 1 && !pressed)
		click_state = 0;

	for(ei = ui_menu_exclusiveness.begin(); ei != ui_menu_exclusiveness.end(); ei++)
	{
		if((*ei).second)
			exclusive = TRUE;
	}
	
	ui_cursor_x += relx;
	ui_cursor_y += rely;

//	logstream() << "UI_Draw: " << ui_cursor_x << " " << ui_cursor_y << " " << pressed << "\n";

	//TEMP 640
	if(ui_cursor_x < 0) ui_cursor_x = 0; if(ui_cursor_x > 640) ui_cursor_x = 640;
	if(ui_cursor_y < 0) ui_cursor_y = 0; if(ui_cursor_y > 480) ui_cursor_y = 480;

	for(mi = ui_active_menus.begin(); mi != ui_active_menus.end(); mi++)
	{
		if((*mi).second)
			if(menu = UI_GetElemByName((*mi).first))
			{
				menu->Draw(0, 0);
				menu->Mouse(0, 0, ui_cursor_x / 640, ui_cursor_y / 480, click);
			}

		if(exclusive)
			//draw cursor :)
			if(!ui_cursor)
				C_DrawText("+", ui_cursor_x, ui_cursor_y);
			else
			{
				math::FloatRectangle	dst;

				dst.left(ui_cursor_x / 640 - 0.025);
				dst.top(ui_cursor_y / 480 - 0.033);
				dst.width(0.05);
				dst.height(0.066);
				
				C_DrawImage(ui_cursor, NULL, &dst);
			}
	}
}

void cmd_menu(void)
{
	ui_menu_iterator		mi;

	if(ui_active_menus.find("ingame_menu") == ui_active_menus.end())
	{
		UI_ActivateMenu("ingame_menu", TRUE);
		return;
	}

	if(ui_active_menus["ingame_menu"])
	{
		UI_DeactivateMenu("ingame_menu");
		return;
	}

	UI_ActivateMenu("ingame_menu", TRUE);
}

void UI_Init(void)
{
	Localfile	lf;

	cvars::addCmd("menu", cmd_menu);

	lf.open("basehaste/ui/test.gui", "r");

	if(lf.streamIsOk())
		UI_Load(lf);

	lf.close();

	lf.open("basehaste/ui/cursor.jpg", "rb");
	if(lf.streamIsOk())
	ui_cursor = C_LoadImage(lf, Pixelformat_RGB888);

	lf.close();

	UI_ActivateMenu("asmenu", TRUE);
}

