//
// Created by amburkoff on 11.05.19.
//

#include "msg_handlers.h"

namespace server {

    int handleMsgPause(Server *server, User *user, struct MsgPause *msg) {
        Room *room = server->getRoom(msg->room);
        std::cout << msg;
        room->pause();
    }

    int handleMsgResume(Server *server, User *user, struct MsgResume *msg) {
        Room *room = server->getRoom(msg->room);
        std::cout << msg;
        room->resume();
    }

    int handleMsgSource(Server *server, User *user, struct MsgSource *msg) {
        Room *room = server->getRoom(msg->room);
        std::cout << msg;
        room->sendSource(user);
    }

}
