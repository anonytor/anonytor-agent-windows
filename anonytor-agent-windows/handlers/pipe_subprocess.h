#pragma once
#include <windows.h>
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#include <string>

using namespace std;
#define BUFSIZE 4096 


void CreateChildProcess(const TCHAR* szCmdLine);
void WriteToPipe(void);
void ReadFromPipe(void);
string pipeStart(string command);
