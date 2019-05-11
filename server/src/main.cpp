//
// Created by amburkoff on 11.05.19.
//

#include "../include/Server.h"

using namespace server;

int main(int args, char **argv) {
    Server *server = Server::getServer();

    std::cout << server;

    server->openSocket();

    fprintf(stderr, "Goodbye :)");

    return 0;
}