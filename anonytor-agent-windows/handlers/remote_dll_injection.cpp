#include "pch.h"

#include "runtime/utils.h"

//find the address of LoadLibrary (it's the same accross all processes)
static void* getLoadLibraryAddr()
{
	HMODULE hKernel32 = GetModuleHandle(L"kernel32.dll");
	if (hKernel32 == NULL)
	{
		ErrorExit(TEXT("GetModuleHandle"));
	}
	LPVOID llBaseAddress = (LPVOID)GetProcAddress(hKernel32, "LoadLibraryW");
	if (llBaseAddress == NULL)
	{
		ErrorExit(TEXT("GetProcAddress"));
	}
	printf("[+] LoadLibrary base address is: 0x%p\n", llBaseAddress);
	return llBaseAddress;
}

bool injectDll(DWORD pid, LPCWSTR dll_path)
{
	HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	if (hProcess == NULL)
	{
		//printf("[-] Couldn't open process, exiting...\n");
		//return -1;
		ErrorExit(TEXT("OpenProcess"));
	}
	printf("[+] Process handle: 0x%p\n", hProcess);
	
	//allocate memory in target process
	LPVOID lpBaseAddress = (LPVOID)VirtualAllocEx(hProcess, NULL, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (lpBaseAddress == NULL)
	{
		ErrorExit(TEXT("VirtualAllocEx"));
	}
	printf("[+] Allocated memory address in target process is: 0x%p\n", lpBaseAddress);

	//write DLL name to target process
	SIZE_T* lpNumberOfBytesWritten = 0;
	BOOL resWPM = WriteProcessMemory(hProcess, lpBaseAddress, dll_path, wcslen(dll_path) * 2 + 1, lpNumberOfBytesWritten);
	if (!resWPM)
	{
		ErrorExit(TEXT("WriteProcessMemory"));
	}
	printf("[+] DLL name is written to memory of target process\n");


	//start remote thread in target process
	HANDLE hThread = NULL;
	DWORD ThreadId = 0;
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)getLoadLibraryAddr(), lpBaseAddress, 0, (LPDWORD)(&ThreadId));
	if (hThread == NULL)
	{
		ErrorExit(TEXT("CreateRemoteThread"));
	}
	printf("[+] Successfully started DLL in target process\n");
	if (ThreadId != 0)
	{
		printf("[+] Injected thread id: %ul for pid: %ul\n", ThreadId, pid);
	}
	return true;
}