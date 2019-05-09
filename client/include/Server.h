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
    Server(std::string &baseurl);

private:
    std::string baseurl;

    std::vector<Room*> knownRooms;
};


#endif //ATMOSPHERECLIENT_SERVER_H
