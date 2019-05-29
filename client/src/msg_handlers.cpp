//
// Created by amburkoff on 11.05.19.
//

#include <Client.h>
#include "msg_handlers.h"

void handleMsgSource(Server *server, struct MsgSource *msg) {
    Room* room = server->getRoom(msg->room);

    room->setSource(msg->source);
}

void handleMsgPause(Server *server, struct MsgPause *msg) {
    Room* room = server->getRoom(msg->room);
    Player* player = Client::getClient()->getPlayer();

    player->lock();
    room->playing = false;
    player->notifyUpdatePlayState();
    player->unlock();
}

void handleMsgResume(Server *server, struct MsgResume *msg) {
    Room *room = server->getRoom(msg->room);
    Player *player = Client::getClient()->getPlayer();

    player->lock();
    room->playing = true;
    player->notifyUpdatePlayState();
    player->unlock();
}

void handleMsgSeek(Server *server, struct MsgSeek *msg) {
    Room *room = server->getRoom(msg->room);
    Player *player = Client::getClient()->getPlayer();

    player->lock();
    room->progress = msg->percentage;
    player->notifyUpdatePlayState();
    player->unlock();
}

void handleMsgListRooms(Server *server, struct MsgListRooms *msg) {
    int n_rooms = (msg->size - (long)(((struct MsgListRooms*)0)->ids)) / sizeof(int);

    server->clearRooms();

    for (int i = 0; i < n_rooms; i++) {
        server->addRoom(new Room(msg->ids[i], server));
    }

    if (server->getCurrentRoom() == NULL && n_rooms > 0)
        server->selectRoom(server->getRoom(msg->ids[0]));
}

void handleMsgListMovies(Server *server, struct MsgListMovies *msg) {
    int n_movies = (msg->size - (long)(((struct MsgListRooms*)0)->ids)) / sizeof(int);

    Room* room = server->getRoom(msg->room);
    room->clearMovies();

    for (int i = 0; i < n_movies; i++) {
        room->addMovie(new Movie(msg->ids[i]));
    }

    room->selectMovie(msg->selected);
}
