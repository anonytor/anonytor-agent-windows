#pragma once


typedef struct exe {
	int ID;					//进程ID 
	wchar_t name[MAX_PATH];	//进程名 
	struct exe* next;
}EXE;
void printList(EXE* list);//打印链表 
EXE* creatList();//创建链表 
EXE* update(EXE* list);//更新
void ShowError();