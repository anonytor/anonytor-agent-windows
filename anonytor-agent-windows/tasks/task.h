//
// Created by 16977 on 2020/9/10.
//

#ifndef C_SOCKET_CLIENT_BASE_H
#define C_SOCKET_CLIENT_BASE_H

#include <string>

using namespace std;

class task {
public:
    task() = default;
    virtual void execute() {};
};

#endif //C_SOCKET_CLIENT_BASE_H
