//
// Created by alex on 08.05.19.
//

#ifndef ATMOSPHERESERVER_USER_H
#define ATMOSPHERESERVER_USER_H


#include <netinet/in.h>

namespace server {

    class User {
    public:
        User(int sock_fd, struct sockaddr_in* addr);

        int getSockFD();

    private:
        friend void* listenerFunc(void* user_p);

        int sock_fd;
        struct sockaddr_in addr{};
        pthread_t thread;
    };

}

#endif //ATMOSPHERESERVER_USER_H
