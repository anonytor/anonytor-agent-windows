//
// Created by 16977 on 2020/9/10.
//

#include "pch.h"
#include "TransferConnection.h"

#include <utility>
using namespace Connection;
TransferConnection::TransferConnection(
        const char *hostAddr, int port,  string hostID, string key, bool type,string taskID):
        BaseConnection(hostAddr, port, hostID, move(key), type) {
    this->TaskID=std::move(taskID);
}
