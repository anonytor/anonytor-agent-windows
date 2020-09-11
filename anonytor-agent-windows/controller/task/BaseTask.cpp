//
// Created by 16977 on 2020/9/10.
//
#include "pch.h"
#include "BaseTask.h"
#include <windows.h>
#include <cppbase64/base64.h>
#include "../definition/Slice.h"
using namespace std;

char *BaseTask::retrieveError() {
    return this->errorDescription;
}

bool BaseTask::validateParam() {
    return true;
}

bool BaseTask::execute() {
    Sleep(1000);
    char r[] = "taskResult";
    this->result = r;
    this->resultLen = strlen(r);
    return true;
}

bool BaseTask::checkIfTransConnNeeded() {
    return false;
}

void BaseTask::getSerializedResult(Slice*ret) {
    basic_string<char> b64 = base64_encode(reinterpret_cast<const unsigned char *>(this->result), this->resultLen,
                                           false);
    char *data=(char*)b64.c_str();
    ret->append(Slice(data,strlen(data),strlen(data)));
}

bool BaseTask::sendResultThroughConn(SOCKET socket) {
    char d[] = "TaskFinished";
    int sentLen = send(socket, d, strlen(d), 0);
    return sentLen == strlen(d);

}

bool BaseTask::checkFinish() {
    return true;
}