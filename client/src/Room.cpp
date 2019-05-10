//
// Created by alex on 10.05.19.
//

#include "../include/Room.h"
#include "../../include/Message.h"

void Room::pause()
{
    struct Message* msg = (struct Message*) malloc(256);

    struct MsgPause msgPause = {1};

    sendMessage(server->getSockFD(), (struct Message*) &msgPause);
}

int Room::getID() {
    return id;
}

Room::Room(int id, Server* server) {
    this->id = id;
    this->server = server;

    server->addRoom(this);
}
