#pragma once

bool get_system();
bool elevate_execute(DWORD pid, LPCWSTR executable_path, LPWSTR command_line);