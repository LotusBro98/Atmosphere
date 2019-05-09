//
// Created by alex on 08.05.19.
//

#include "../include/Client.h"

void Client::addServer(Server *server) {
    knownServers.push_back(server);
}
