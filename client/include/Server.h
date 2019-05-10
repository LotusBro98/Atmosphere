//
// Created by alex on 10.05.19.
//

#ifndef ATMOSPHERECLIENT_SERVER_H
#define ATMOSPHERECLIENT_SERVER_H


#include <string>
#include <vector>

class Server;

#include "Room.h"

class Server {
public:
    Server(std::string baseurl);

    int getSockFD();

    void addRoom(Room* room);

    Room* getRoom(int id);

    bool isConnected();

private:
    std::string baseurl;

    int sock_fd;
    int openSocket(short port = 23443);

    bool connected = false;

    std::vector<Room*> knownRooms;
};


#endif //ATMOSPHERECLIENT_SERVER_H
