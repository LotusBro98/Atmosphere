//
// Created by alex on 10.05.19.
//

#include <cstring>
#include <Client.h>
#include "../include/Room.h"
#include "../../include/Message.h"

void Room::pause()
{
    struct MsgPause msg = {};
    msg.room = this->id;

    std::cout << "<- " << &msg;

    server->sendMessage((struct Message*) &msg);
}

void Room::play()
{
    struct MsgResume msg = {};
    msg.room = this->id;

    std::cout << "<- " << &msg;

    server->sendMessage((struct Message*) &msg);
}

int Room::getID() {
    return id;
}

Room::Room(int id, Server* server) {
    this->id = id;
    this->server = server;
}

void Room::setSource(char *source) {
    strcpy(this->source, source);

    Player* player = Client::getClient()->getPlayer();

    player->notifyUpdateSource();
}

char* Room::getSource() {
    return source;
}

void Room::requestUpdateSource() {
    char* source = "";
    int size = 1;
    size += (long)(((struct MsgSource*)0)->source);

    struct MsgSource* msg = (struct MsgSource*) malloc(size);

    msg->type = MSG_SOURCE;
    msg->size = size;
    msg->room = this->id;
    strcpy(msg->source, source);

    std::cout << "<- " << msg;

    server->sendMessage((struct Message*) msg);

    free(msg);
}

void Room::seek(float percentage) {
    struct MsgSeek msg = {};
    msg.room = this->id;
    msg.percentage = percentage;

    std::cout << "<- " << &msg;

    server->sendMessage((struct Message*) &msg);
}



