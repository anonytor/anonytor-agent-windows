#pragma once

extern HINSTANCE hInst;                                // 当前实例
extern wchar_t executable_path[MAX_PATH + 1];
extern wchar_t module_path[MAX_PATH + 1];
extern bool stand_alone;
extern HINSTANCE hModule;
extern wchar_t* commandline;
