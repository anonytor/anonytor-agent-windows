#include "pch.h"

#include "runtime/utils.h"

void setStratupReg()
{
	//根键、子键名称和到子键的句柄
	HKEY hRoot = HKEY_CURRENT_USER;
	HKEY hKey;//打开指定子键
	DWORD dwDisposition = REG_OPENED_EXISTING_KEY;
	//如果不存在就创建
	LONG lRet = RegCreateKeyEx(
		hRoot,
		L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		0,
		NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
		NULL,
		&hKey,
		&dwDisposition
	);
	if (lRet != ERROR_SUCCESS)
	{
		ShowError(lRet);
		return;
	}
	puts("open registry success");
	//得到当前执行文件的文件名（包含路径）
	wchar_t szModule[MAX_PATH];
	GetModuleFileName(NULL, szModule, MAX_PATH);
	std::wcout << szModule << std::endl;
	//创建一个新的键值，设置键值数据为文件
	lRet = RegSetValueEx(
		hKey,
		L"System32Tool",
		0,
		REG_SZ,
		(BYTE*)szModule,
		wcslen(szModule) * 2
	);
	if (lRet != ERROR_SUCCESS)
	{
		ShowError(lRet);
		return;
	}
	puts("reg set value success");
	//关闭子键句柄
	RegCloseKey(hKey);
}
