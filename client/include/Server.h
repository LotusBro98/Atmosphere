//
// Created by alex on 10.05.19.
//

#ifndef ATMOSPHERECLIENT_SERVER_H
#define ATMOSPHERECLIENT_SERVER_H


#include <string>
#include <vector>
#include <list>

class Server;

#include "Room.h"

class Server {
public:
    Server(std::string baseurl);

    int getSockFD();

    void addRoom(Room* room);
    void clearRooms();
    std::list<Room* > getRooms();

    Room* getRoom(int id);
    Room* getCurrentRoom();

    void selectRoom(Room* room);

    bool isConnected();

    friend void* listenToServer(void* server_ptr);

    void listenToServerAsync();
    void disconnect();

    int sendMessage(struct ::Message* msg);
    int recvMessage(struct ::Message* msg);

    void updateRooms();

private:
    std::string baseurl;

    int sock_fd;
    pthread_mutex_t sock_mutex;

    int openSocket(short port = 23443);

    bool connected = false;

    pthread_t thread;

    std::list<Room*> knownRooms;
};


#endif //ATMOSPHERECLIENT_SERVER_H
