//
// Created by amburkoff on 11.05.19.
//

#ifndef PLAYER_TEST_MSG_HANDLERS_H
#define PLAYER_TEST_MSG_HANDLERS_H

#include "../include/Server.h"
#include "../../include/Message.h"

void handleMsgSource(Server* server, struct MsgSource* msg);

void handleMsgPause(Server *server, struct MsgPause *msg);

void handleMsgResume(Server *server, struct MsgResume *msg);

#endif //PLAYER_TEST_MSG_HANDLERS_H
