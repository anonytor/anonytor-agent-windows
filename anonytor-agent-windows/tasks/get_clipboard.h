//
// Created by mukeran on 2020/9/12.
//

#ifndef WECHATLUCKYMONEY_GET_CLIPBOARD_H
#define WECHATLUCKYMONEY_GET_CLIPBOARD_H

#include "task.h"

class get_clipboard : public task {
private:
    string taskId;
public:
    get_clipboard (string);
    void execute();
};


#endif //WECHATLUCKYMONEY_GET_CLIPBOARD_H
