#include "pch.h"
#include "../connection/client.h"
#include <fstream>
#include<cpp-base64/base64.h>
#include "get_file_content.h"
#include "config.h"

get_file_content::get_file_content(string _taskId, string _path) {
	taskId = std::move(_taskId);
	path = std::move(_path);
}

char buffer_[100001];
void get_file_content::execute() {
	client client(config::host, config::port);
	client.connect();
	client.handshake(config::id, config::key, transfer, taskId);
	ifstream in(path, ios::binary);
	if(!in.is_open())
	{
		client::instance->sendPacket(response{ errorInExecution, taskId, "" }.dump());
		return;
	}
	in.seekg(0, ios::end);
	int size = in.tellg();
	in.seekg(0, ios::beg);
	while (size > 0) {
		in.read(buffer_, 100000);
		int send = 100000;
		if (size < 100000)
			send = size;
		size -= send;
		client.sendRaw(buffer_, send);
	}
	client::instance->sendPacket(response{ finished, taskId, "" }.dump());
}