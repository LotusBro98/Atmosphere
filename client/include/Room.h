//
// Created by alex on 10.05.19.
//

#ifndef ATMOSPHERECLIENT_ROOM_H
#define ATMOSPHERECLIENT_ROOM_H

class Room;

#include "Server.h"

class Room {
public:
    Room(int id);

    void connectToStream();

    void pause();
    void resume();
    void seek(float percentage);

private:
    Server* server;
    int id;
};


#endif //ATMOSPHERECLIENT_ROOM_H
