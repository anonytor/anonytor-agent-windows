//
// Created by 16977 on 2020/9/10.
//

#include "pch.h"
#include "ControlConnection.h"

#include <utility>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace Connection;
using namespace std;

ControlConnection::ControlConnection(
        const char *hostAddr, int port,string hostID, string key, bool type)
        : BaseConnection(hostAddr, port, hostID, move(key), type) {

}

bool ControlConnection::sendResponse(Response response) {
    json j;
    j["task_id"] = response.TaskID;
    j["task_status"] = response.TaskStatus;
    j["Data"] = response.Data;
    string s = j.dump();
    Slice sl = Slice((char *) s.data(), s.length(), s.length());
    return this->basicSend(sl)==-1;
    return this->recvOK();
}
bool  ControlConnection::recvTask(BaseTask*bt) {
    Request r=Request();
    int status=this->recvRequest(&r);
    if (status==-1){
        return false;
    }
    // TODO: return different types of tasks depend on r.CMD
     bt->taskId=r.TaskID;
    bt->param=bt->param;
    return true;

}
void ControlConnection::handleTask(BaseTask bt) {
    Response r(bt.taskId,bt.TaskReceived,"");
    // validate
    bool status=bt.validateParam();
    if (!status){
        r.TaskStatus=bt.TaskErrorInExecution;
        r.Data=bt.retrieveError();
        this->sendResponse(r);
    }
    status=bt.execute();
    if (!status){
        r.TaskStatus=bt.TaskErrorInExecution;
        this->sendResponse(r);
    }
    status=bt.checkIfTransConnNeeded();
    if (status){
        r.TaskStatus=bt.TaskWantRetrieveThroughTrans;
        this->sendResponse(r);
        // TODO: Establish trans connection and send data
    }else{
        // send directly
        r.TaskStatus=bt.TaskWantRetrieveThroughCtrl;
        Slice result=Slice("",0,0);
        bt.getSerializedResult(&result);
        r.Data=(char*)malloc(sizeof(char)*(result.getLen()+1));
        memcpy(r.Data,result.buffer,result.getLen());
        r.Data[result.getLen()]=0;
        this->sendResponse(r);
    }
    r.TaskStatus=bt.TaskFinished;
    delete[] r.Data;
    r.Data = new char[1];
    r.Data[0] = 0;
    this->sendResponse(r);

}
