//
// Created by 16977 on 2020/9/10.
//

#ifndef C_SOCKET_CLIENT_SOCKET_H
#define C_SOCKET_CLIENT_SOCKET_H

#include <winsock2.h>
#include <string>
#include "../definition/Slice.h"
#include "../definition/Request.h"

using namespace std;
using namespace Protocol;
namespace Connection {
    const int Control= 0;
const int Transfer =1;
    const int DEFAULT_BUFFER_LENGTH = 256;
    class BaseConnection {
    protected:
        SOCKADDR_IN ServerAddr;
        SOCKET Socket;
        Slice Buffer = Slice("", 0, DEFAULT_BUFFER_LENGTH);
        string HostID;
        string Key;
        int Type;
        string TaskID;

        //methods
        int basicSend(Slice) const;

        int  basicRecv(Slice *sl);

        int readAndAppendToBuffer();
        void sendHandshake();
        int recvRequest(Request *);
        bool recvOK();
        int OK();
        int Reset();


    public:
        BaseConnection(const char *hostAddr, int port, const string &hostID, string key, int type);

        bool Connect();


    };

}


#endif //C_SOCKET_CLIENT_SOCKET_H
