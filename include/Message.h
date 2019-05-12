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
    MSG_PAUSE, MSG_RESUME, MSG_SEEK, MSG_SOURCE, MSG_LIST_ROOMS, MSG_SET_ROOM, MSG_LIST_MOVIES
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

inline std::ostream& operator<< (std::ostream& os, struct MsgSeek* msg)
{
    os << "MsgSeek: " << msg->room << "; " << msg->percentage << "\n";
    return os;
}

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

struct MsgListRooms
{
    int type = MSG_LIST_ROOMS;
    int size;
    int ids[];
};

inline std::ostream& operator<< (std::ostream& os, struct MsgListRooms* msg)
{
    int n_rooms = (msg->size - (long)(((struct MsgListRooms*)0)->ids)) / sizeof(int);

    os << "MsgListRooms: [ ";

    for (int i = 0; i < n_rooms; i++)
    {
        os << msg->ids[i] << " ";
    }

    os << "]\n";

    return os;
}

struct MsgSetRoom
{
    int type = MSG_SET_ROOM;
    int size = sizeof(struct MsgSetRoom);
    int room;
};

inline std::ostream& operator<< (std::ostream& os, struct MsgSetRoom* msg)
{
    os << "MsgSetRoom: " << msg->room << "\n";
    return os;
}




struct MsgListMovies
{
    int type = MSG_LIST_MOVIES;
    int size;
    int room;
    int selected;
    int ids[];
};

inline std::ostream& operator<< (std::ostream& os, struct MsgListMovies* msg)
{
    int n_movies = (msg->size - (long)(((struct MsgListMovies*)0)->ids)) / sizeof(int);

    os << "MsgListMovies: room: " << msg->room << "; movies: [ ";

    for (int i = 0; i < n_movies; i++)
    {
        os << msg->ids[i] << " ";
    }

    os << "]\n";

    return os;
}

#endif //ATMOSPHERE_MESSAGE_H
