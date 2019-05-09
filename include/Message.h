//
// Created by alex on 10.05.19.
//

#ifndef ATMOSPHERE_MESSAGE_H
#define ATMOSPHERE_MESSAGE_H

struct Message
{
    int type;
    int size;
    char data[];
};

enum MSG_TYPES
{
    MSG_PAUSE, MSG_RESUME, MSG_SEEK
};

struct MsgPause
{
    int type = MSG_PAUSE;
    int size = sizeof(*this);
    int room;
};

struct MsgResume
{
    int type = MSG_RESUME;
    int size = sizeof(*this);
    int room;
};

struct MsgSeek
{
    int type = MSG_SEEK;
    int size = sizeof(*this);
    int room;
    float percentage;

};

#endif //ATMOSPHERE_MESSAGE_H
