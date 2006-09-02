#include "common.h"
#include "cvars.hh"
#include "keys.h"

#include <windows.h>
#include <string.h>

static std::string	commands[256];
static char			key_to_scan[256][32];

static ion_uint8	scantokey[128];

void cmd_bind(void)
{
	int		i;
	string	cmd;

	if(cvars::numArgs() < 3)
	{
		logstream() << "Usage: bind key command\n";
		return;
	}

	for(i = 2; i < cvars::numArgs(); i++)
	{
		cmd += cvars::getArg(i).STLstring();

		if(i != cvars::numArgs() - 1)
			cmd += " ";
	}

	for(i = 0; i < 256; i++)
		if(cvars::getArg(1).compare(key_to_scan[i], false))
		{
			commands[i] = cmd;
			return;
		}

	commands[i] = "";
}

void IN_Init(void)
{
	long	i;

	for(i = 0; i < 256; i++)
		commands[i] = "";

	for(i = 0; i < 256; i++)
		GetKeyNameText(i << 16, key_to_scan[i], 32);

	cvars::addCmd("bind", cmd_bind);

	for (i=0 ; i<128 ; i++)
		scantokey[i] = ' ';

	//FIXME: that's english keyboards only
	scantokey[  1] = K_ESCAPE;
	scantokey[  2] = '1';
	scantokey[  3] = '2';
	scantokey[  4] = '3';
	scantokey[  5] = '4';
	scantokey[  6] = '5';
	scantokey[  7] = '6';
	scantokey[  8] = '7';
	scantokey[  9] = '8';
	scantokey[ 10] = '9';
	scantokey[ 11] = '0';
	scantokey[ 12] = '-';
	scantokey[ 13] = '=';
	scantokey[ 14] = K_BACKSPACE;
	scantokey[ 15] = K_TAB;
	scantokey[ 16] = 'q';       
	scantokey[ 17] = 'w';       
	scantokey[ 18] = 'e';       
	scantokey[ 19] = 'r';       
	scantokey[ 20] = 't';       
	scantokey[ 21] = 'y';       
	scantokey[ 22] = 'u';       
	scantokey[ 23] = 'i';       
	scantokey[ 24] = 'o';       
	scantokey[ 25] = 'p';       
	scantokey[ 26] = '[';
	scantokey[ 27] = ']';
	scantokey[ 28] = K_ENTER;
	scantokey[ 29] = K_CTRL; //left
	scantokey[ 30] = 'a';
	scantokey[ 31] = 's';       
	scantokey[ 32] = 'd';       
	scantokey[ 33] = 'f';       
	scantokey[ 34] = 'g';       
	scantokey[ 35] = 'h';       
	scantokey[ 36] = 'j';       
	scantokey[ 37] = 'k';       
	scantokey[ 38] = 'l';       
	scantokey[ 39] = ';';
	scantokey[ 40] = '\'';
	scantokey[ 41] = '`';
	scantokey[ 42] = K_SHIFT; //left
	scantokey[ 43] = '\\';
	scantokey[ 44] = 'z';       
	scantokey[ 45] = 'x';  
	scantokey[ 46] = 'c';
	scantokey[ 47] = 'v';       
	scantokey[ 48] = 'b';
	scantokey[ 49] = 'n';       
	scantokey[ 50] = 'm';       
	scantokey[ 51] = ',';
	scantokey[ 52] = '.';
	scantokey[ 53] = '/';
	scantokey[ 54] = K_SHIFT; //right
	scantokey[ 55] = '*'; //keypad
	scantokey[ 56] = K_ALT; //left
	scantokey[ 57] = ' ';
	// 58 caps lock
	scantokey[ 59] = K_F1;
	scantokey[ 60] = K_F2;
	scantokey[ 61] = K_F3;
	scantokey[ 62] = K_F4;
	scantokey[ 63] = K_F5;
	scantokey[ 64] = K_F6;
	scantokey[ 65] = K_F7;
	scantokey[ 66] = K_F8;
	scantokey[ 67] = K_F9;
	scantokey[ 68] = K_F10;
	// 69 numlock
	// 70 scrollock
	scantokey[ 71] = K_KP_HOME;
	scantokey[ 72] = K_KP_UPARROW;
	scantokey[ 73] = K_KP_PGUP;
	scantokey[ 74] = K_KP_MINUS;
	scantokey[ 75] = K_KP_LEFTARROW;
	scantokey[ 76] = K_KP_5;
	scantokey[ 77] = K_KP_RIGHTARROW;
	scantokey[ 79] = K_KP_END;
	scantokey[ 78] = K_KP_PLUS;
	scantokey[ 80] = K_KP_DOWNARROW;
	scantokey[ 81] = K_KP_PGDN;
	scantokey[ 82] = K_KP_INS;
	scantokey[ 83] = K_KP_DEL;
	// 84 to 86 not used
	scantokey[ 87] = K_F11;
	scantokey[ 88] = K_F12;
	// 89 to 95 not used
	scantokey[ 96] = K_KP_ENTER; //keypad enter
	scantokey[ 97] = K_CTRL; //right
	scantokey[ 98] = K_KP_SLASH;
	scantokey[ 99] = K_F12; // print screen, bind to screenshot by default
	scantokey[100] = K_ALT; // right

	scantokey[101] = K_PAUSE; // break
	scantokey[102] = K_HOME;
	scantokey[103] = K_UPARROW;
	scantokey[104] = K_PGUP;
	scantokey[105] = K_LEFTARROW;
	scantokey[106] = K_RIGHTARROW;
	scantokey[107] = K_END;
	scantokey[108] = K_DOWNARROW;
	scantokey[109] = K_PGDN;
	scantokey[110] = K_INS;
	scantokey[111] = K_DEL;

	scantokey[119] = K_PAUSE;
}

void IN_Shut(void)
{
}

void IN_Key_Event(int scancode, ion_uint8 state)
{
	std::string	cmd;

//	if(key == 164) return;

	if(state)
		C_ForwardKey(scantokey[scancode]);

	cmd = commands[scancode];

	if(C_IsActive() && cmd != "toggle_console")
		return;

	if(cmd[0] == '+' && (state == 0)) cmd[0] = '-';
	if(cmd[0] == '-' && (state == 1)) cmd[0] = '+';

	if(cmd[0] == '+' || cmd[0] == '-' || state)
		cvars::execString(cmd);
}
