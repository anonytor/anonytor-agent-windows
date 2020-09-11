//
// Created by 16977 on 2020/9/10.
//

#ifndef CSOCK_RESPONSE_H
#define CSOCK_RESPONSE_H


#include <string>

using namespace std;
namespace Protocol{

    const int TaskInitialized = 0;
// 发送前
    const int TaskSent = 1;
// 发送后
    const int TaskReceived = 2;
// 二选一，是否提升连接
    const int TaskWantRetrieveThroughCtrl = 3;
    const int TaskWantRetrieveThroughTrans = 4;
// 传输连接建立
    const int TransConnEstablished = 5;
// 完成
    const int TaskFinished = 6;

    const int TaskErrorInExecution = 7;

    class Response {
    public:
        Response(string taskID,int taskStatus,const char* data);
        string TaskID;
        int TaskStatus;
        char *Data;
    };
}

#endif //CSOCK_RESPONSE_H
