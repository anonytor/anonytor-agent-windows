#include "pch.h"

#include "runtime/utils.h"

// 全局变量:

WCHAR* GetClipboardText()
{
	if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		puts("Not text in clipboard !");
		return NULL;
	}
	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
	{
		puts("GetClipboardText: OpenClipboard failed !");
		ShowError();
	}

	// Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData(CF_UNICODETEXT);
	if (hData == nullptr) 
	{
		puts("GetClipboardText: GetClipboardData failed !");
		ShowError();
	}

	// Lock the handle to get the actual text pointer
	WCHAR* pszText = static_cast<wchar_t*>(GlobalLock(hData));
	if (pszText == nullptr)
	{
		puts("GetClipboardText: GlobalLock failed !");
		ShowError();
		return NULL;
	}

	// Release the lock
	GlobalUnlock(hData);

	// Release the clipboard
	CloseClipboard();

	return pszText;
}

const WCHAR* GetClipboardTextHelper()
{
	WCHAR* tmp = GetClipboardText();
	if (tmp == NULL)
	{
		return L"";
	}
	return tmp;
}