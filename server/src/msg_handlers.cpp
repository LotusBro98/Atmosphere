//
// Created by amburkoff on 11.05.19.
//

#include "msg_handlers.h"

namespace server {

    int handleMsgPause(Server *server, User *user, struct MsgPause *msg) {
        Room *room = server->getRoom(msg->room);

        room->pause();
    }

    int handleMsgResume(Server *server, User *user, struct MsgResume *msg) {
        Room *room = server->getRoom(msg->room);

        room->resume();
    }

    int handleMsgSource(Server *server, User *user, struct MsgSource *msg) {
        Room *room = server->getRoom(msg->room);

        room->sendSource(user);
    }

    int handleMsgSeek(Server *server, User *user, struct MsgSeek *msg) {
        Room *room = server->getRoom(msg->room);

        room->seek(msg->percentage);
    }

    int handleMsgListRooms(Server *server, User *user, struct MsgListRooms *msg) {
        int n_rooms = server->getRooms().size();
        int size = sizeof(int) * n_rooms + (long)(((struct MsgListRooms*)0)->ids);

        struct MsgListRooms* res = (struct MsgListRooms*) malloc(size);
        res->type = MSG_LIST_ROOMS;
        res->size = size;

        int i = 0;
        for (Room* room : server->getRooms())
        {
            res->ids[i] = room->getId();
        }

        std::cout << "<- " << res;
        user->sendMessage((struct Message*) res);

        free(res);
    }

    int handleMsgSetRoom(Server *server, User *user, struct MsgSetRoom *msg) {
        Room *room = server->getRoom(msg->room);

        room->addUser(user);

        room->sendSource(user);
    }

}
