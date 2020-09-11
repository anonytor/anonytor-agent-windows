//
// Created by 16977 on 2020/9/10.
//
#include "pch.h"
#include "Handshake.h"
#include <string>
using namespace std;
Handshake::Handshake(string hostID, string key, bool type, string taskID) {
    this->HostID=hostID;
    this->TaskID=taskID;
    this->Type=type;
    this->Key=key;
}