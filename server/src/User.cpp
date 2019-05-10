//
// Created by alex on 08.05.19.
//

#include "../include/User.h"

using namespace server;

User::User(int sock_fd, struct sockaddr_in *addr) {
    this->sock_fd = sock_fd;
    this->addr = *addr;
    this->thread = thread;
}

int User::getSockFD() {
    return sock_fd;
}
