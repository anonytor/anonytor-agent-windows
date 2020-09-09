#pragma once

BOOL GetSystem(LPCWSTR self_cmd);
BOOL ElevateExecute(DWORD pid, LPCWSTR executable_path, LPWSTR command_line);
BOOL ElevateExecute(LPCWSTR executable_path, LPWSTR command_line);
BOOL ElevateSelf();
BOOL ElevateSelf(DWORD pid);
