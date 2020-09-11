//
// Created by 16977 on 2020/9/10.
//

#ifndef C_SOCKET_CLIENT_HANDSHAKE_H
#define C_SOCKET_CLIENT_HANDSHAKE_H

#include <string>
using namespace std;

class Handshake {
public:
   string HostID;
   string Key;
   int Type;
   string TaskID;

   Handshake(string hostID, string key, bool type, string taskId);

};


#endif //C_SOCKET_CLIENT_HANDSHAKE_H
