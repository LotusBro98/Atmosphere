//
// Created by alex on 10.05.19.
//

#include "../include/Message.h"
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstring>


int sendMessage(int fd, struct Message* msg)
{
    errno = 0;

    char * buf = (char*) msg;
    char * end = buf + msg->size;
    char * p = buf;
    int rc;

    //printf("Writing to %d\n", fd);

    while ((p < end) && (rc = write(fd, p, end - p)) != 0)
    {
        if (errno) {
            perror("Failed to send message");
            return errno;
        }

        p += rc;
    }

    if (p < end)
    {
        perror("Message sent partially.");
        return 1;
    }

    return 0;
}

int recvMessage(int fd, struct Message* msg)
{
    errno = 0;
    int rc;

    if ((rc = read(fd, &msg->type, sizeof(int))) <= 0) {
        fprintf(stderr, "Socket disconnected while reading msg type %d: %s (%d)\n", rc, strerror(errno), errno);
        return 1;
    }
    //fprintf(stderr, "Got msg type: %d\n", msg->type);

    if ((rc = read(fd, &msg->size, sizeof(int))) <= 0) {
        fprintf(stderr, "Socket disconnected while reading msg size %d: %s (%d)\n", rc, strerror(errno), errno);
        return 1;
    }
    //fprintf(stderr, "Got msg size: %d\n", msg->size);

    char * buf = msg->data;
    char * end = (char*)msg + msg->size;
    char * p = buf;

    while ((p < end) && ((rc = read(fd, p, end - p)) != 0))
    {
        if (errno) {
            perror("Failed to recieve message");
            return errno;
        }

        p += rc;
    }

    if (p < end)
    {
        fprintf(stderr, "Message received partially. %d left; %s (%d)\n", end - p, strerror(errno), errno);
        return 1;
    }

    return 0;
}