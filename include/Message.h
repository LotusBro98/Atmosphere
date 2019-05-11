//
// Created by alex on 10.05.19.
//

#ifndef ATMOSPHERE_MESSAGE_H
#define ATMOSPHERE_MESSAGE_H

#include <iostream>

#define MAX_MSG_SIZE 4096

struct Message
{
    int type;
    int size;
    char data[];
};

inline std::ostream& operator<< (std::ostream& os, struct Message* msg)
{
    os << "Msg: " << msg->type << "; " << msg->size << "; " << msg->data << "\n";
    return os;
}

int sendMessage(int fd, struct Message* msg);
int recvMessage(int fd, struct Message* msg);

enum MSG_TYPES
{
    MSG_PAUSE, MSG_RESUME, MSG_SEEK, MSG_SOURCE, MSG_ROOMS
};

struct MsgPause
{
    int type = MSG_PAUSE;
    int size = sizeof(*this);
    int room;
};

inline std::ostream& operator<< (std::ostream& os, struct MsgPause* msg)
{
    os << "MsgPause: " << msg->room << "\n";
    return os;
}

struct MsgResume
{
    int type = MSG_RESUME;
    int size = sizeof(*this);
    int room;
};

inline std::ostream& operator<< (std::ostream& os, struct MsgResume* msg)
{
    os << "MsgResume: " << msg->room << "\n";
    return os;
}

struct MsgSeek
{
    int type = MSG_SEEK;
    int size = sizeof(*this);
    int room;
    float percentage;

};

struct MsgSource
{
    int type = MSG_SEEK;
    int size;
    int room;
    char source[];
};

inline std::ostream& operator<< (std::ostream& os, struct MsgSource* msg)
{
    os << "MsgSource: " << msg->room << "; " << msg->source << "\n";
    return os;
}

struct MsgRooms
{
    int type = MSG_ROOMS;
};

#endif //ATMOSPHERE_MESSAGE_H
