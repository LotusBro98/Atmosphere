//
// Created by alex on 08.05.19.
//

#include "../include/Client.h"

void Client::addServer(Server *server) {
    knownServers.push_back(server);
}

Client::Client() {
    loadServers();
}

void Client::loadServers() {
    Server* server = new Server("127.0.0.1");
    addServer(server);
}

Server* Client::getServer(int index) {
    return knownServers.at(index);
}
