#pragma once
#ifndef WECHATLUCKYMONEY_LISTEN_KEYBOARD_H
#define WECHATLUCKYMONEY_LISTEN_KEYBOARD_H

#include "task.h"
#include <string>

class listen_keyboard : public task {
private:
    string taskId;
    string hostId;
public:
    listen_keyboard(string);
    void execute();
};


#endif //WECHATLUCKYMONEY_LISTEN_KEYBOARD_H