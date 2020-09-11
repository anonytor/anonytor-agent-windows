//
// Created by mukeran on 2020/9/12.
//

#ifndef WECHATLUCKYMONEY_RESPONSE_H
#define WECHATLUCKYMONEY_RESPONSE_H

#include <string>

using namespace std;

enum task_status {
    initialized,
    sent,
    received,
    wantRetrieveThroughTrans,
    transConnEstablished,
    finished,
    errorInExecution
};

struct response {
    task_status status;
    string task_id;
    string data;
    string dump() {
        json j{
            {"task_status", static_cast<int>(status)},
            {"task_id", task_id},
            {"data", data}
        };
        return j.dump();
    }
};


#endif //WECHATLUCKYMONEY_RESPONSE_H
