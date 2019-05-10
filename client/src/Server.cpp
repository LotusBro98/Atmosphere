//
// Created by alex on 10.05.19.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#include "../include/Server.h"

Server::Server(std::string baseurl) {
    this->baseurl = baseurl;

    if (openSocket())
    {
        errno = 0;
        connected = false;
    } else {
        connected = true;
    }
}

int Server::openSocket(short port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    //addr.sin_addr.s_addr = inet_aton(this->baseurl.data(), &addr.sin_addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*) &addr, sizeof(addr));

    if (errno)
    {
        fprintf(stderr, "Failed to connect: %s (%d)\n", strerror(errno), errno);
        return errno;
    }

    this->sock_fd = sock;
    return 0;
}

int Server::getSockFD()
{
    return sock_fd;
}

void Server::addRoom(Room *room) {
    knownRooms.push_back(room);
}

Room* Server::getRoom(int id) {
    for (Room* room : knownRooms)
    {
        if (room->getID() == id)
            return room;
    }
    return NULL;
}

bool Server::isConnected() {
    return connected;
}
