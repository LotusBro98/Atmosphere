//
// Created by alex on 08.05.19.
//

#include "../include/Client.h"

Client::Client() {
    connectToServer();

    player = new Player();
    player->init(0, NULL);
}

Server* Client::getServer() {
    return server;
}

void Client::connectToServer() {
    server = new Server("127.0.0.1");
    //server = new Server("172.20.20.155");
    //server = new Server("192.168.43.115");
    //server = new Server("192.168.43.186");
    server->listenToServerAsync();
}

Player *Client::getPlayer() {
    return player;
}

Client::~Client() {
    player->destroy();
    server->disconnect();

    delete player;
    delete server;
}

void Client::playerMain() {
    player->start();
}
