#pragma once
#include "pch.h"
#include "pipe_subprocess.h"

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;
HANDLE g_hInputFile = NULL;

string cmdBuffer1 = "";

void ErrorExit1(PCTSTR);
bool IsNotPrint(char c)
{
    return !isprint(c) && c != 0;
}
string pipeStart(string command)
{
    SECURITY_ATTRIBUTES saAttr;
    printf("\n->Start of parent execution.\n");

    // Set the bInheritHandle flag so pipe handles are inherited. 

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's STDOUT. 

    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
        ErrorExit1(TEXT("StdoutRd CreatePipe"));

    // Ensure the read handle to the pipe for STDOUT is not inherited.


    // Create a pipe for the child process's STDIN. 

    if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
        ErrorExit1(TEXT("Stdin CreatePipe"));

    // Ensure the write handle to the pipe for STDIN is not inherited. 


    // Create the child process. 
    string cmd = "C:\\Windows\\System32\\cmd.exe";
    cmd.append(" /c ");
    cmd.append(command);
    TCHAR wc[MAX_PATH];
    _stprintf_s(wc, MAX_PATH, _T("%S"), cmd.c_str());
    
    CreateChildProcess(wc);
    // Get a handle to an input file for the parent. 
    // This example assumes a plain text file and uses string output to verify data flow. 


    // g_hInputFile = GetStdHandle(STD_INPUT_HANDLE);
    //
    // if (g_hInputFile == INVALID_HANDLE_VALUE)
    //     ErrorExit1(TEXT("CreateFile"));

    // Write to the pipe that is the standard input for a child process. 
    // Data is written to the pipe's buffers, so it is not necessary to wait
    // until the child process is running before writing data.
    cmdBuffer1 = "";
    ReadFromPipe();
    //
    // for (;;)
    // {
    //     WriteToPipe();
    //     ReadFromPipe();
    // }

    // Read from pipe that is the standard output for child process. 

    // printf("\n->Contents of child process STDOUT:\n\n");

    // printf("\n->End of parent execution.\n");

    // The remaining open handles are cleaned up when this process terminates. 
    // To avoid resource leaks in a larger application, close handles explicitly.
    // 
    CloseHandle(g_hChildStd_OUT_Wr);
    CloseHandle(g_hChildStd_IN_Rd);
    CloseHandle(g_hChildStd_IN_Wr);
    CloseHandle(g_hChildStd_OUT_Rd);
    std::remove_if(cmdBuffer1.begin(), cmdBuffer1.end(), IsNotPrint);
    return cmdBuffer1;
}

void CreateChildProcess(const TCHAR* szCmdline)
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

    // Set up members of the PROCESS_INFORMATION structure. 

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    // Set up members of the STARTUPINFO structure. 
    // This structure specifies the STDIN and STDOUT handles for redirection.

    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.hStdInput = g_hChildStd_IN_Rd;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process. 

    bSuccess = CreateProcess(NULL,
        (LPWSTR)szCmdline,     // command line 
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION 

     // If an error occurs, exit the application. 
    if (!bSuccess)
        ErrorExit1(TEXT("CreateProcess"));
    else
    {

        // Close handles to the stdin and stdout pipes no longer needed by the child process.
        // If they are not explicitly closed, there is no way to recognize that the child process has ended.

  
    	if (WaitForSingleObject(piProcInfo.hProcess, 10000) == WAIT_FAILED)
    	{
            CHAR chBuf[BUFSIZE];
            strcpy(chBuf, "timeout");
            DWORD dwWritten;
            // WriteFile(g_hChildStd_OUT_Wr, chBuf, 7, &dwWritten, NULL);
    	}
        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);
        // Close handles to the child process and its primary thread.
        // Some applications might keep these handles to monitor the status
        // of the child process, for example. 
    }
}

void WriteToPipe(void)

// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data. 
{
    DWORD dwRead, dwWritten;
    CHAR chBuf[BUFSIZE];
    BOOL bSuccess = FALSE;

    for (;;)
    {
        bSuccess = ReadFile(g_hInputFile, chBuf, BUFSIZE, &dwRead, NULL);
        if (!bSuccess || dwRead == 0) break;
        cmdBuffer1.append(chBuf);
        bSuccess = WriteFile(g_hChildStd_IN_Wr, chBuf, dwRead, &dwWritten, NULL);
        if (!bSuccess) break;
        break;
    }

    // Close the pipe handle so the child process stops reading. 

    //if (!CloseHandle(g_hChildStd_IN_Wr))
    //    ErrorExit(TEXT("StdInWr CloseHandle"));
}

void ReadFromPipe(void)

// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT. 
// Stop when there is no more data. 
{
    DWORD dwRead, dwWritten;
    CHAR chBuf[BUFSIZE];
    BOOL bSuccess = FALSE;


    for (;;)
    {
        DWORD total_available_bytes;
        if (FALSE == PeekNamedPipe(g_hChildStd_OUT_Rd,
            0,
            0,
            0,
            &total_available_bytes,
            0))
        {
            ErrorExit1(TEXT("PeekFailed"));
            return;
        }
        else if (total_available_bytes > 0)
        {
            bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
            cmdBuffer1.append(chBuf);
            if (!bSuccess) ErrorExit1(TEXT("ReadFileFailed"));
        }
        else {
            break;
        }
    }
}

void ErrorExit1(PCTSTR lpszFunction)

// Format a readable error message, display a message box, 
// and exit from the application.
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(1);
}