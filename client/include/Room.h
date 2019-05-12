//
// Created by alex on 10.05.19.
//

#ifndef ATMOSPHERECLIENT_ROOM_H
#define ATMOSPHERECLIENT_ROOM_H

class Room;

#include "../../include/Message.h"
#include "Server.h"
#include "Movie.h"


class Room {
public:
    Room(int id, Server* server);

    void connectToStream();

    void pause();
    void play();
    void seek(float percentage);

    void requestUpdateSource();
    void requestUpdateMovies();

    void setSource(char* source);
    char* getSource();

    bool playing;
    float progress;

    int getID();

private:
    Server* server;
    int id;
    char source[MAX_MSG_SIZE];

    std::list<Movie*> movies;
    Movie* currentMovie;
};


#endif //ATMOSPHERECLIENT_ROOM_H
