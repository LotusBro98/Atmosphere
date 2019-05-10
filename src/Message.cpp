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

    while ((p < end) && (rc = write(fd, p, end - p) != 0))
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

    if (read(fd, &msg->type, sizeof(int)) <= 0)
        return 1;

    if (read(fd, &msg->size, sizeof(int)) <= 0)
        return 1;

    char * buf = msg->data;
    char * end = (char*)msg + msg->size;
    char * p = buf;
    int rc;

    while ((p < end) && (rc = read(fd, p, end - p) != 0))
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