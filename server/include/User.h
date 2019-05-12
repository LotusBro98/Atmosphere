//
// Created by alex on 08.05.19.
//

#ifndef ATMOSPHERESERVER_USER_H
#define ATMOSPHERESERVER_USER_H

namespace server {
    class User;
}

#include <netinet/in.h>
#include "../../include/Message.h"
#include "Room.h"

namespace server {

    class User {
    public:
        User(int sock_fd, struct sockaddr_in* addr);

        int getSockFD();

        int sendMessage(struct ::Message* msg);
        int recvMessage(struct ::Message* msg);

        Room* getCurrentRoom();
        void setCurrentRoom(Room* room);

        pthread_t thread;

    private:
        friend void* listenerFunc(void* user_p);

        int sock_fd;
        struct sockaddr_in addr{};

        pthread_mutex_t mutex;

        Room* currentRoom;
    };

}

#endif //ATMOSPHERESERVER_USER_H
