//
// Created by 16977 on 2020/9/10.
//

#ifndef CSOCK_NOTICE_H
#define CSOCK_NOTICE_H

#endif //CSOCK_NOTICE_H
 namespace Notice{
    const char LogInitialized[]="logger initialized";
    const char ClientSocketUsable[]="client socket is usable";
    const char ServerConnected[]="connected to server";
}


namespace Notice{
    const char SocketInitError[]="can't initialize socket";
    const char ServerConnectError[]= "can't connect to server";
    const char ClientSocketVersionError[]="client socket's version is incorrect";
    const char SocketRecvError[]="can't recv correct Data from socket";

}
