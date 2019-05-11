//
// Created by alex on 10.05.19.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "../include/Server.h"
#include "../../include/Message.h"
#include "msg_handlers.h"


using namespace server;

namespace server {

    void *listenerFunc(void *user_p) {
        struct Message *msg = (struct Message *) malloc(MAX_MSG_SIZE);

        User *user = (User *) user_p;
        user->thread = pthread_self();

        Server *server = Server::getServer();
        server->addUser(user);

        while (server->isAlive()) {
            if (user->recvMessage(msg))
                break;

            switch (msg->type)
            {
                case MSG_PAUSE:
                    std::cout << (struct MsgPause*)msg;
                    handleMsgPause(server, user, (struct MsgPause*)msg);
                    break;
                case MSG_RESUME:
                    std::cout << (struct MsgResume*)msg;
                    handleMsgResume(server, user, (struct MsgResume*)msg);
                    break;
                case MSG_SOURCE:
                    std::cout << (struct MsgSource*)msg;
                    handleMsgSource(server, user, (struct MsgSource*)msg);
                    break;
                case MSG_SEEK:
                    std::cout << (struct MsgSeek*)msg;
                    handleMsgSeek(server, user, (struct MsgSeek*)msg);
                    break;
                case MSG_LIST_ROOMS:
                    std::cout << (struct MsgListRooms*)msg;
                    handleMsgListRooms(server, user, (struct MsgListRooms*)msg);
                    break;
                case MSG_SET_ROOM:
                    std::cout << (struct MsgSetRoom*)msg;
                    handleMsgSetRoom(server, user, (struct MsgSetRoom*)msg);
                    break;
                default:
                    std::cout << msg;
            }
        }

        free(msg);
    }

}

int Server::openSocket(short port) {
    errno = 0;

    int main_sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int TRUE = 1;
    setsockopt(main_sock, SOL_SOCKET, SO_REUSEADDR, &TRUE, sizeof(TRUE));

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