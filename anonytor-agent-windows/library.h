#pragma once


#ifdef ANONYTORAGENTWINDOWS_EXPORTS
#define LIBRARY_API __declspec(dllexport)
#else
#define LIBRARY_API __declspec(dllimport)
#endif

extern "C" LIBRARY_API void CALLBACK EntryPoint(HWND hwnd, HINSTANCE hinst, LPCSTR lpszCmdLine, int nCmdShow);

