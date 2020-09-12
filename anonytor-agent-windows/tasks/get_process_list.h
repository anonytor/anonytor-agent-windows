#pragma once
#ifndef WECHATLUCKYMONEY_PROCESS_LIST_H
#define WECHATLUCKYMONEY_PROCESS_LIST_H

#include "task.h"
#include <string>

class get_process_list : public task {
private:
    string taskId;
    string process;
public:
    get_process_list(string);
    void execute();
};


#endif //WECHATLUCKYMONEY_PROCESS_LIST_H