//
// Created by alex on 10.05.19.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "../include/Server.h"
#include "../../include/Message.h"

using namespace server;

void* server::listenerFunc(void *user_p) {
    struct Message* msg = (struct Message*) malloc(256);

    User* user = (User*) user_p;
    user->thread = pthread_self();

    Server* server = Server::getServer();
    server->addUser(user);

    while (server->isAlive())
    {
        if (recvMessage(user->sock_fd, msg))
            break;
        std::cout << msg;
    }
}

int Server::openSocket(short port) {
    errno = 0;

    int main_sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(main_sock, (struct sockaddr*) &addr, sizeof(addr));
    listen(main_sock, 10);

    if (errno)
    {
        perror("Failed to open socket\n");
        return errno;
    }

    __socklen_t addr_len;
    while (isAlive())
    {
        int fd = accept(main_sock,(struct sockaddr*) &addr, &addr_len);
        if (errno)
        {
            perror("Failed to accept connection\n");
            continue;
        }

        User* user = new User(fd, &addr);

        pthread_create(&this->listener_thread, NULL, server::listenerFunc, user);
    }

    //pthread_join(...);

    return 0;
}