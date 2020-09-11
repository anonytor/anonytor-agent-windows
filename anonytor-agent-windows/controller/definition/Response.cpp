//
// Created by 16977 on 2020/9/10.
//
#include "pch.h"
#include "Response.h"

#include <utility>

using namespace Protocol;

Response::Response(string taskID, int taskStatus, const char *data) {
    this->TaskID = std::move(taskID);
    this->TaskStatus = taskStatus;
    int len = strlen(data);
    this->Data = new char[len + 1];
    memcpy(this->Data, data, sizeof(char) * (len + 1));
}
