//
// Created by alex on 10.05.19.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "../include/Server.h"

using namespace server;

void* server::listenerFunc(void *server_p) {
    Server* server = (Server*) server_p;

    struct sockaddr addr;
    socklen_t addr_len;
    while (server->isAlive())
    {
        int fd = accept4(server->main_sock, &addr, &addr_len, SOCK_NONBLOCK);
        if (errno)
        {
            perror("Failed to accept connection\n");
            return (void*) (long) errno;
        }

        User* user = new User(fd, &addr);

        server->addUser(user);
    }
}

int Server::openSocket(short port) {
    main_sock = socket(AF_INET, SOCK_STREAM, 0);

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

    pthread_create(&this->listener_thread, NULL, server::listenerFunc, this);

    return 0;
}