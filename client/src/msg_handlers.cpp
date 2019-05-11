//
// Created by amburkoff on 11.05.19.
//

#include <Client.h>
#include "msg_handlers.h"

void handleMsgSource(Server *server, struct MsgSource *msg) {
    Room* room = server->getRoom(msg->room);

    std::cout << "=> " << msg;

    room->setSource(msg->source);
}

void handleMsgPause(Server *server, struct MsgPause *msg) {
    Room* room = server->getRoom(msg->room);
    Player* player = Client::getClient()->getPlayer();

    std::cout << "=> " << msg;

    room->playing = false;
    player->notifyUpdatePlayState();
}

void handleMsgResume(Server *server, struct MsgResume *msg) {
    Room *room = server->getRoom(msg->room);
    Player *player = Client::getClient()->getPlayer();

    std::cout << "=> " << msg;

    room->playing = true;
    player->notifyUpdatePlayState();
}
