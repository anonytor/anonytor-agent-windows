#pragma once


typedef struct my_proc {
	DWORD ID;					//进程ID 
	WCHAR name[MAX_PATH];	//进程名 
	struct my_proc* next;
} MY_PROC;
void PrintProcList(MY_PROC* list);//打印链表 
MY_PROC* CreatProcList();//创建链表 
void DestroyProcList(MY_PROC* list);