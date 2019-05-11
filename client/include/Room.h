//
// Created by alex on 10.05.19.
//

#ifndef ATMOSPHERECLIENT_ROOM_H
#define ATMOSPHERECLIENT_ROOM_H

class Room;

#include "Server.h"
#include "../../include/Message.h"

class Room {
public:
    Room(int id, Server* server);

    void connectToStream();

    void pause();
    void play();
    void seek(float percentage);

    void requestUpdateSource();

    void setSource(char* source);
    char* getSource();

    bool playing;
    float progress;

    int getID();

private:
    Server* server;
    int id;
    char source[MAX_MSG_SIZE];



};


#endif //ATMOSPHERECLIENT_ROOM_H
