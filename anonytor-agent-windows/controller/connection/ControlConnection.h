//
// Created by 16977 on 2020/9/10.
//

#ifndef CSOCK_CONTROLCONNECTION_H
#define CSOCK_CONTROLCONNECTION_H

#include "BaseConnection.h"
#include <string>
#include "../definition/Response.h"
#include "../task/BaseTask.h"
namespace Connection {
    class ControlConnection : public BaseConnection {
    private:
    public:
        ControlConnection(const char* hostAddr, int port, string hostID, string key, bool type);
        bool  recvTask(BaseTask*);
        bool sendResponse(Response);
        void handleTask(BaseTask bt);
    };
}


#endif //CSOCK_CONTROLCONNECTION_H
