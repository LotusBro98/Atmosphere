//
// Created by alex on 08.05.19.
//

#ifndef ATMOSPHERESERVER_USER_H
#define ATMOSPHERESERVER_USER_H


#include <netinet/in.h>
#include "../../include/Message.h"

namespace server {

    class User {
    public:
        User(int sock_fd, struct sockaddr_in* addr);

        int getSockFD();

        int sendMessage(struct ::Message* msg);
        int recvMessage(struct ::Message* msg);

    private:
        friend void* listenerFunc(void* user_p);

        int sock_fd;
        struct sockaddr_in addr{};
        pthread_t thread;
        pthread_mutex_t mutex;
    };

}

#endif //ATMOSPHERESERVER_USER_H
