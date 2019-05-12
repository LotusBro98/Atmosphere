//
// Created by alex on 10.05.19.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#include "../include/Server.h"
#include "../../include/Message.h"
#include "msg_handlers.h"

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
    pthread_mutex_init(&sock_mutex, NULL);
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

void* listenToServer(void* server_ptr) {
    Server* server = (Server*) server_ptr;
    struct Message* msg = (struct Message*) malloc(MAX_MSG_SIZE);

    while (server->connected)
    {
        if (server->recvMessage(msg)) {
            break;
        }

        switch (msg->type)
        {
            case MSG_SOURCE:
                std::cout << "=> " << (struct MsgSource*) msg;
                handleMsgSource(server, (struct MsgSource*) msg);
                break;
            case MSG_PAUSE:
                std::cout << "=> " << (struct MsgPause*) msg;
                handleMsgPause(server, (struct MsgPause*) msg);
                break;
            case MSG_RESUME:
                std::cout << "=> " << (struct MsgResume*) msg;
                handleMsgResume(server, (struct MsgResume*) msg);
                break;
            case MSG_SEEK:
                std::cout << "=> " << (struct MsgSeek*) msg;
                handleMsgSeek(server, (struct MsgSeek*) msg);
                break;
            case MSG_LIST_ROOMS:
                std::cout << "=> " << (struct MsgListRooms*) msg;
                handleMsgListRooms(server, (struct MsgListRooms*) msg);
                break;
            default:
                std::cout << "=> " << msg;
                break;
        }
    }

    free(msg);

    fprintf(stderr, "Lost connection with server. Exiting.\n");
    exit(1);
    return NULL;
}

void Server::listenToServerAsync() {
    pthread_create(&thread, NULL, listenToServer, this);
}

void Server::disconnect() {
    connected = false;
    close(sock_fd);
    pthread_cancel(thread);
    pthread_join(thread, NULL);
}

Room *Server::getCurrentRoom() {
    return currentRoom;
}

int Server::sendMessage(struct ::Message *msg) {
    //printf("Lock ");
    pthread_mutex_lock(&sock_mutex);
    //printf("Lock\n");
    int rc = ::sendMessage(sock_fd, msg);
    pthread_mutex_unlock(&sock_mutex);
    //printf("Unlock\n");
    return rc;
}

int Server::recvMessage(struct ::Message *msg) {
    //pthread_mutex_lock(&sock_mutex);
    int rc = ::recvMessage(sock_fd, msg);
    //pthread_mutex_unlock(&sock_mutex);
    return rc;
}

void Server::clearRooms() {
    knownRooms.clear();
}

void Server::updateRooms() {
    int size = (long)(((struct MsgListRooms*)0)->ids);
    MsgListRooms* msg = (struct MsgListRooms*) malloc(size);

    msg->type = MSG_LIST_ROOMS;
    msg->size = size;

    std::cout << "<- " << msg;

    sendMessage((struct Message*) msg);
    free(msg);
}

std::list<Room *> Server::getRooms() {
    return knownRooms;
}

void Server::selectRoom(Room* room) {
    MsgSetRoom msg;

    msg.room = room->getID();

    std::cout << "<- " << &msg;

    currentRoom = room;
    sendMessage((struct Message*) &msg);
}
