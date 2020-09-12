#include "pch.h"
#include "../connection/client.h"
#include "get_process_list.h"
#include "handlers/process_list.h"
#include <TlHelp32.h>
get_process_list::get_process_list(string _taskId) {
	taskId = std::move(_taskId);
}

string wide_Char_To_Multi_Byte(wchar_t* pWCStrKey)
{
	//��һ�ε���ȷ��ת�����ֽ��ַ����ĳ��ȣ����ڿ��ٿռ�
	int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
	char* pCStrKey = new char[pSize + 1];
	//�ڶ��ε��ý�˫�ֽ��ַ���ת���ɵ��ֽ��ַ���
	WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
	pCStrKey[pSize] = '\0';
	string pKey = pCStrKey;
	return pKey;
}
void get_process_list::execute() {
	MY_PROC* list=CreatProcList();
	PROCESSENTRY32 pe32;			//�������̿��յĽṹ�� 
	pe32.dwSize = sizeof(PROCESSENTRY32);		//���ô�С
	int i = 0;
	while (list != NULL)
	{
		//printf("ID:%-16u", list->ID);
		//process[i].ID = list->ID;
		process.append("ID: ");
		process.append(to_string(long long(list->ID)));
		//std::wcout << L"name: " << list->name << std::endl;
		//wcsncpy(process[i].name, list->name , wcslen(pe32.szExeFile) + 1);
		string tmp = wide_Char_To_Multi_Byte(list->name);
		process.append("  Name:  ");
		process.append(tmp);
		process.append("\n");
		list = list->next;
		i++;
	}
	client::instance->sendPacket(response{ finished, taskId, process }.dump());
}