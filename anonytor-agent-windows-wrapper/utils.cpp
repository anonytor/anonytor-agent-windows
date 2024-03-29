#include "framework.h"

LPWSTR lstrcat_heap(LPCWSTR str1, LPCWSTR str2)
{
	size_t len1 = (size_t)lstrlen(str1) * 2;
	size_t len2 = (size_t)lstrlen(str2) * 2;
	LPWSTR dest = (LPWSTR) malloc(len1 + len2 + 1);
	if (dest == NULL)
	{
		return NULL;
	}
	lstrcpy(dest, str1);
	lstrcat(dest, str2);
	return dest;
}

void ConsoleInit()
{
	if (AllocConsole())
	{
		FILE* pCout;
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		SetConsoleTitle(L"Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
}

void ShowError()
{
	// Get last error.
	DWORD err = GetLastError();

	// Translate ErrorCode to String.
	LPTSTR Error = 0;
	if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		err,
		0,
		(LPTSTR)&Error,
		0,
		NULL) == 0)
	{
		// Failed in translating.
	}

	// Display message.
	MessageBox(NULL, Error, L"Error", MB_OK | MB_ICONWARNING);

	// Free the buffer.
	if (Error)
	{
		::LocalFree(Error);
		Error = 0;
	}
}

void ShowError(DWORD err)
{
	// Translate ErrorCode to String.
	LPTSTR Error = 0;
	if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		err,
		0,
		(LPTSTR)&Error,
		0,
		NULL) == 0)
	{
		// Failed in translating.
	}

	// Display message.
	MessageBox(NULL, Error, L"Error", MB_OK | MB_ICONWARNING);

	// Free the buffer.
	if (Error)
	{
		::LocalFree(Error);
		Error = 0;
	}
}


void ErrorExit(const wchar_t* message)
{
	// MessageBox(NULL, message, L"Error", MB_OK | MB_ICONWARNING);
	exit(0);
}

void ShowMessage(wchar_t* message)
{
	MessageBox(NULL, message, L"Error", MB_OK | MB_ICONWARNING);
}