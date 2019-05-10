//
// Created by alex on 10.05.19.
//

#ifndef ATMOSPHERECLIENT_ROOM_H
#define ATMOSPHERECLIENT_ROOM_H

class Room;

#include "Server.h"

class Room {
public:
    Room(int id, Server* server);

    void connectToStream();

    void pause();
    void resume();
    void seek(float percentage);

    int getID();

private:
    Server* server;
    int id;
};


#endif //ATMOSPHERECLIENT_ROOM_H
