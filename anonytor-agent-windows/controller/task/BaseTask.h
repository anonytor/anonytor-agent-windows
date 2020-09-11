//
// Created by 16977 on 2020/9/10.
//

#ifndef C_SOCKET_CLIENT_BASE_H
#define C_SOCKET_CLIENT_BASE_H

#include <string>
#include <utility>
#include <winsock2.h>
#include "../definition/Slice.h"
using namespace std;

class BaseTask {

private:
    char * errorDescription;
    char * result;
    int resultLen;
public:
    const static int TaskInitialized = 0;
    // 发送前
    const static int  TaskSent = 1;
    // 发送后
    const static int  TaskReceived = 2;
    // 二选一，是否提升连接
    const static int  TaskWantRetrieveThroughCtrl = 3;
    const static int  TaskWantRetrieveThroughTrans = 4;
    // 传输连接建立
    const static int  TaskTransConnEstablished = 5;
    // 完成
    const static int  TaskFinished = 6;

    const static int  TaskErrorInExecution = 7;


    string taskId;
    string param;
    string transType;
    BaseTask(){
        this->taskId="";
        this->param="";
        this->transType="";
    }
    BaseTask(string taskId, string param) {
        this->taskId = std::move(taskId);
        this->param = std::move(param);
    };

    // Lifetimes
    //retrieveError 返回当前错误的描述。
    // 理想情况下，当其他方法返回的 bool=false 时
    // 调用此方法获取错误描述，然后上层发送给服务端
    virtual char* retrieveError();

    virtual bool validateParam();

    virtual bool execute();

    virtual bool checkIfTransConnNeeded();

    virtual void getSerializedResult(Slice*sl);

    virtual bool sendResultThroughConn(SOCKET socket);

    virtual bool checkFinish();


};


#endif //C_SOCKET_CLIENT_BASE_H
