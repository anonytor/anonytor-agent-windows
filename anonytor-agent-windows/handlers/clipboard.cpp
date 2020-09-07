#include "pch.h"

#include "runtime/utils.h"

// 全局变量:

wchar_t* GetClipboardText()
{
	if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		puts("clipboard not text!!");
		return NULL;
	}
	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
	{
		ShowError();
	}

	// Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData(CF_UNICODETEXT);
	if (hData == nullptr) 
	{
		ShowError();
	}

	// Lock the handle to get the actual text pointer
	wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
	if (pszText == nullptr)
	{
		ShowError();
		return NULL;
	}

	// Release the lock
	GlobalUnlock(hData);

	// Release the clipboard
	CloseClipboard();

	return pszText;
}


std::ofstream ofs;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
}
