#pragma once
#ifndef WECHATLUCKYMONEY_PIPE_SUBPROCESS_H
#define WECHATLUCKYMONEY_PIPE_SUBPROCESS_H

#include "task.h"
#include <string>

class exec_command : public task {
private:
    string taskId;
    string command;
public:
    exec_command(string,string);
    void execute();
};


#endif //WECHATLUCKYMONEY_PIPE_SUBPROCESS_H