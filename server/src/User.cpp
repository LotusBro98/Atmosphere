//
// Created by alex on 08.05.19.
//

#include <pthread.h>
#include "../include/User.h"
#include "../../include/Message.h"

using namespace server;

User::User(int sock_fd, struct sockaddr_in *addr) {
    this->sock_fd = sock_fd;
    this->addr = *addr;
    this->thread = thread;
    pthread_mutex_init(&mutex, NULL);
}

int User::getSockFD() {
    return sock_fd;
}


int User::sendMessage(struct ::Message *msg) {
    //printf("Lock ");
    pthread_mutex_lock(&mutex);
    //printf("Lock\n");
    int rc = ::sendMessage(sock_fd, msg);
    pthread_mutex_unlock(&mutex);
    //printf("Unlock\n");
    return rc;
}

int User::recvMessage(struct ::Message *msg) {
    //pthread_mutex_lock(&mutex);
    int rc = ::recvMessage(sock_fd, msg);
    //pthread_mutex_unlock(&mutex);
    return rc;
}
