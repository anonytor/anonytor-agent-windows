﻿#include "pch.h"

#include <tlhelp32.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <tchar.h>


#include "process_list.h"

EXE* creatList() //创建链表
{
	//std::wcout << L"cout hello!还好" << std::endl;
	//wprintf(L"aaa调用失败！ggg\n");
	EXE* head = NULL, * now = NULL, * last = NULL;
	PROCESSENTRY32 pe32;			//创建进程快照的结构体 
	pe32.dwSize = sizeof(PROCESSENTRY32);		//设置大小 
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);		//获取系统中所有的进程快照 
	if (hProcessSnap == INVALID_HANDLE_VALUE)			//调用函数失败返回 INVALID_HANDLE_VALUE
	{
		printf("failed\n");
		return 0;
	}
	BOOL bmore = Process32First(hProcessSnap, &pe32);	//调用成功 获取第一个进程信息 获取成功则进入循环 
	while (bmore)
	{
		now = (EXE*)malloc(sizeof(struct exe));				//为当前的指针分配空间 
		now->ID = pe32.th32ProcessID;			//将进程ID赋值到节点中 
		wcsncpy(now->name, pe32.szExeFile, wcslen(pe32.szExeFile)+1);		//将进程名赋值到节点中 

		if (head == NULL) 	//创建的为第一个节点 
		{
			head = now;
			last = now;
		}
		else
		{
			last->next = now;
			last = now;
		}
		bmore = Process32Next(hProcessSnap, &pe32);			//获取下一个进程信息 
	}
	last->next = NULL;									//将最后一个节点的next指针置为空 
	CloseHandle(hProcessSnap);								//关闭句柄 
	return head;
}

void printList(EXE* list)//创建打印列表的函数 
{
	EXE* head = list;
	while (list != NULL)
	{
		printf("ID:%-16u", list->ID);
		std::wcout << L"name: " << list->name << std::endl;
		list = list->next;
	}
	list = head;		//重置为头结点 
	printf("\n");
}

void readFile(char * path)
{
	//path = "C:\\Users\\Miracle\\Desktop\\V.txt";
	//FILE* fp = fopen(path, "wb+");
	FILE* fp = fopen("C:\\Users\\Miracle\\Desktop\\V.txt", "wb+");

	printf("%s\n", path);
	printf("success");
	fclose(fp);
}
char* GetFilename(char* p) //得到一个路径的纯文件名
{
	int x = strlen(p);
	char ch = '\\';
	char* q = strrchr(p, ch);
	return q;
}
