//
// Created by amburkoff on 11.05.19.
//

#ifndef ATMOSPHERE_MSG_HANDLERS_H
#define ATMOSPHERE_MSG_HANDLERS_H

#include "../include/Server.h"
#include "../include/User.h"

namespace server {

    int handleMsgPause(Server *server, User *user, struct MsgPause *msg);

    int handleMsgResume(Server *server, User *user, struct MsgResume *msg);

    int handleMsgSource(Server *server, User *user, struct MsgSource *msg);

    int handleMsgSeek(Server *server, User *user, struct MsgSeek *msg);

    int handleMsgListRooms(Server *server, User *user, struct MsgListRooms *msg);

    int handleMsgSetRoom(Server *server, User *user, struct MsgSetRoom *msg);

    int handleMsgListRooms(Server *server, User *user, struct MsgListRooms *msg);

}

#endif //ATMOSPHERE_MSG_HANDLERS_H
