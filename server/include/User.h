//
// Created by alex on 08.05.19.
//

#ifndef ATMOSPHERESERVER_USER_H
#define ATMOSPHERESERVER_USER_H


#include <netinet/in.h>

namespace server {

    class User {
    public:
        User(int sock_fd, struct sockaddr* addr);

        int getSockFD();

    private:
        int sock_fd;
        struct sockaddr addr;
    };

}

#endif //ATMOSPHERESERVER_USER_H
