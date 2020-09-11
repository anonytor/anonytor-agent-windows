//
// Created by 16977 on 2020/9/10.
//

#include "pch.h"
#include "BaseConnection.h"
#include <iostream>
#include "spdlog/spdlog.h"
#include "../definition/Notice.h"
#include "../definition/Handshake.h"
#include "nlohmann/json.hpp"
#include <cstdlib>
#include "../definition/Slice.h"

#pragma comment(lib, "ws2_32.lib")
using namespace Connection;
using namespace std;
using namespace Protocol;
using json = nlohmann::json;

#pragma warning(disable : 4996)

BaseConnection::BaseConnection(const char *addr, int port, const string &hostID, string key, int type) {
    this->ServerAddr.sin_family = AF_INET;
    this->ServerAddr.sin_addr.S_un.S_addr = inet_addr(addr);
    this->ServerAddr.sin_port = htons(port);
    this->Socket = socket(AF_INET, SOCK_STREAM, 0);
    DWORD timeout = 3 * 1000;
    setsockopt(this->Socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);
    this->Key = move(key);
    this->Type = type;
    this->HostID = hostID;
}

bool BaseConnection::Connect() {
    if (connect(this->Socket, (SOCKADDR *) &this->ServerAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        spdlog::error(Notice::ServerConnectError);
        return false;
    } else {
        spdlog::debug(Notice::ServerConnected);
    }
    this->sendHandshake();
    return this->recvOK();


}

int BaseConnection::basicSend(Slice s) const {
    // get len of string(dataLen)
    char lenStr[512];
    sprintf(lenStr, "%d", s.getLen());
    int numLen = strlen(lenStr);
    int wholeLen = numLen + 1 + s.getLen();
    // alloc buffer
    char *tmp = new char[wholeLen];
    // copy Data to buffer
    memcpy(tmp, lenStr, numLen);
    tmp[numLen] = '\n';
    memcpy(tmp + numLen + 1, s.buffer, s.getLen());
    // send
    cout << tmp;
    int sendLen = send(this->Socket, tmp, wholeLen, 0);
    return sendLen == wholeLen;
}

void BaseConnection::sendHandshake() {
    Handshake hs = Handshake(this->HostID, this->Key, this->Type, this->TaskID);
    json j;
    j["host_id"] = hs.HostID;
    j["key"] = hs.Key;
    j["type"] = hs.Type;
    j["task_id"] = hs.TaskID;
    string s = j.dump();
    Slice sl = Slice((char *) s.data(), s.length(), s.length());
    basicSend(sl);
}

int  BaseConnection::recvRequest(Request* r) {
    Slice s = Slice("",0,0);
    int status=this->basicRecv(&s);
    if (status==-1){
        return -1;
    }
    s.set(s.getLen(), '\0');
    auto data = json::parse(s.buffer);
    r->TaskID = data["task_id"].get<string>();
    r->Cmd = data["cmd"].get<int>();
    r->Param = data["param"].get<string>();
    return 0;

}

bool BaseConnection::recvOK() {
    Request r = Request();
    int status=this->recvRequest(&r);
    if (status==-1){
        return false;
    }
    return r.Cmd == Protocol::OK;

}

int BaseConnection::basicRecv(Slice *ret) {

    int readLength = this->readAndAppendToBuffer();
    if (readLength==-1){
        return -1;
    }
    // get index of \n
    int index = 0;
    while ((this->Buffer).get(index) != '\n' && index < readLength) {
        index++;
    }
    if (index == 0 || index == readLength) {
        spdlog::error(Notice::SocketRecvError);
    }
    char *possibleNum = new char[index];
    for (int i = 0; i < index; i++) {
        possibleNum[i] = this->Buffer.get(i);
    }
    int rawDataLen = atoi(possibleNum);
    int wholeDataLen = rawDataLen + 1 + index;
    while (readLength < wholeDataLen) {
        readLength += readAndAppendToBuffer();
    }
    this->Buffer.get(index + 1, wholeDataLen,ret);
    int remainLen = this->Buffer.getLen() - wholeDataLen;
    if (remainLen == 0) {
        this->Buffer = Slice("", 0, DEFAULT_BUFFER_LENGTH);
    } else {
        this->Buffer.get(wholeDataLen,this->Buffer.getLen(),&this->Buffer);
    }
    return readLength;
}

int BaseConnection::readAndAppendToBuffer() {
    char tmpBuffer[DEFAULT_BUFFER_LENGTH];
    int newlyReadLen = recv(this->Socket, tmpBuffer, DEFAULT_BUFFER_LENGTH, 0);
    if (newlyReadLen==-1){
        return -1;
    }
    Slice sl = Slice(tmpBuffer, newlyReadLen, DEFAULT_BUFFER_LENGTH);
    this->Buffer.append(sl);
    return newlyReadLen;
}

