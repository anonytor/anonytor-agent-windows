//
// Created by mukeran on 2020/9/12.
//

#ifndef WECHATLUCKYMONEY_CLIENT_H
#define WECHATLUCKYMONEY_CLIENT_H
#include <string>
#include "request.h"
#include "response.h"
using namespace std;

const int TEMP_BUFFER_SIZE = 1000;

enum connection {
    control,
    transfer
};

class client {
private:
    int socket;
    string host;
    int port;
    string buffer;
public:
    static client* instance;
    client (string, int);
    void connect();
    void sendPacket(string);
    void sendRaw(const char*, int);
    void sendRaw(string);
    string receivePacket();
    bool handshake(string, string, connection, string="");
    bool isConnected();
};


#endif //WECHATLUCKYMONEY_CLIENT_H
