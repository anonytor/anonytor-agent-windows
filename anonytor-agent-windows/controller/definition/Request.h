//
// Created by 16977 on 2020/9/10.
//

#ifndef CSOCK_REQUEST_H
#define CSOCK_REQUEST_H

#include <string>

using namespace std;
namespace Protocol{
    const int UploadFile = 0;
    const int GetFileContent = 1;
    const int KeyboardInputRecordUpload = 2;
    const int TestUpload = 3;
    const int OK = 4;
    const int Reset = 5;

    class Request {
    public:
        Request(){
            this->Param="";
            this->Cmd=0;
            this->Param="";
        }
        string TaskID;
        int Cmd;
        string Param;
    };
}
//type Request struct {
//    taskId     string     `json:"task_id"`
//    TaskStatus TaskStatus `json:"task_status"`
//    Data       []byte     `json:"Data"`
//}


#endif //CSOCK_REQUEST_H
