//
// Created by 16977 on 2020/9/10.
//

#ifndef CSOCK_TRANSFERCONNECTION_H
#define CSOCK_TRANSFERCONNECTION_H
#include "BaseConnection.h"
namespace Connection{
    class TransferConnection: public BaseConnection{
    public:
        TransferConnection( const char* hostAddr, int port,string hostID, string key, bool type,string taskID);
    };
}



#endif //CSOCK_TRANSFERCONNECTION_H
