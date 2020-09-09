#pragma once

extern HINSTANCE hInst;                                // 当前实例
extern WCHAR executable_path[MAX_PATH + 1];
extern WCHAR module_path[MAX_PATH + 1];
extern BOOL stand_alone;
extern HINSTANCE hModule;
extern WCHAR* commandline;
