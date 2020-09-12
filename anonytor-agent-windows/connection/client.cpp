//
// Created by mukeran on 2020/9/12.
//
#include "pch.h"
#include "client.h"
#include <spdlog/spdlog.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#pragma comment(lib, "Ws2_32.lib")

client* client::instance;
client::client(string host, int port) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    instance = this;
    this->host = host;
    this->port = port;
    this->socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    int timeout = 3 * 1000;
    setsockopt(this->socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);
}

void client::connect() {
    sockaddr_in addr{};
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    InetPtonA(AF_INET, host.data(), &addr.sin_addr);
    addr.sin_port = htons(port);
    if (::connect(this->socket, (sockaddr*) &addr, sizeof(sockaddr)) == -1) {
        spdlog::error("Failed to connect to server");
    } else {
        spdlog::debug("Server connected");
    }
}

void client::sendPacket(string packet) {
    const auto packetLengthString = std::to_string(packet.length());
    const auto totalLength = packetLengthString.length() + packet.length() + 1;
    const string data = packetLengthString + "\n" + packet;
    if (send(this->socket, data.data(), data.length(), 0) == -1) {
        spdlog::error("Failed to send packet to server");
    } else {
        spdlog::debug("Packet " + data + " sent to server");
    }
}

string client::receivePacket() {
    char *tmpBuffer = new char[TEMP_BUFFER_SIZE + 1];
    int received;
    string packet;
    while (true) {
        while ((received = recv(this->socket, tmpBuffer, TEMP_BUFFER_SIZE, 0)) > 0) {
            tmpBuffer[received] = 0;
            buffer += tmpBuffer;
        }
        int pos = buffer.find_first_of('\n');
        if (pos == string::npos)
            continue;
        int length = stoi(buffer.substr(0, pos));
        if (length > buffer.length() - (pos + 1)) continue;
        packet = buffer.substr(pos + 1, length);
        buffer = buffer.substr(length + pos + 1);
        break;
    }
    return packet;
}

bool client::handshake(string hostId, string key, connection type, string taskId) {
    json j{
        {"host_id", hostId},
        {"key", key},
        {"type", (int)type}
    };
    if (type == transfer) {
        j.push_back(json{{"task_id", taskId}});
    }
    sendPacket(j.dump());
    string packet = receivePacket();
    auto req = request::parse(packet);
    return req.cmd == cmd_type::OK;
}

bool client::isConnected() {
    return this->socket != -1;
}
