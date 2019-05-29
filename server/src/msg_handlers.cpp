//
// Created by amburkoff on 11.05.19.
//

#include "msg_handlers.h"
#include <unistd.h>

namespace server {

    int handleMsgPause(Server *server, User *user, struct MsgPause *msg) {
        Room *room = server->getRoom(msg->room);

        room->pause();
    }

    int handleMsgResume(Server *server, User *user, struct MsgResume *msg) {
        Room *room = server->getRoom(msg->room);

        room->resume();
    }

    int handleMsgSource(Server *server, User *user, struct MsgSource *msg) {
        Room *room = server->getRoom(msg->room);

        room->sendSource(user);
    }

    int handleMsgSeek(Server *server, User *user, struct MsgSeek *msg) {
        Room *room = server->getRoom(msg->room);

        room->seek(msg->percentage);
    }

    int handleMsgListRooms(Server *server, User *user, struct MsgListRooms *msg) {
        int n_rooms = server->getRooms().size();
        int size = sizeof(int) * n_rooms + (long)(((struct MsgListRooms*)0)->ids);

        struct MsgListRooms* res = (struct MsgListRooms*) malloc(size);
        res->type = MSG_LIST_ROOMS;
        res->size = size;

        int i = 0;
        for (Room* room : server->getRooms())
        {
            res->ids[i] = room->getId();
            i++;
        }

        std::cout << "<- " << res;
        user->sendMessage((struct Message*) res);

        free(res);
    }

    int handleMsgSetRoom(Server *server, User *user, struct MsgSetRoom *msg) {
        Room *room = server->getRoom(msg->room);
        if (user->getCurrentRoom() != NULL) {
            user->getCurrentRoom()->rememberSeek();
        }

        user->setCurrentRoom(room);

        room->sendSource(user);

        sleep(1);

        float curSeek = room->askSeek();
        bool playing = room->isPlaying();

        MsgSeek msg1;
        msg1.type = MSG_SEEK;
        msg1.size = sizeof(MsgSeek);
        msg1.room = room->getId();
        msg1.percentage = curSeek;

        user->sendMessage((struct Message*) &msg1);

        struct MsgPause msg2;
        msg2.type = playing ? MSG_RESUME : MSG_PAUSE;
        msg2.size = sizeof(MsgPause);
        msg2.room = room->getId();

        user->sendMessage((struct Message*) &msg2);
    }

    int handleMsgListMovies(Server *server, User *user, struct MsgListMovies *msg) {
        Room* room = server->getRoom(msg->room);

        int n_movies = room->getMovies().size();
        int size = sizeof(int) * n_movies + (long)(((struct MsgListMovies*)0)->ids);

        struct MsgListMovies* res = (struct MsgListMovies*) malloc(size);
        res->type = MSG_LIST_MOVIES;
        res->size = size;
        res->room = msg->room;

        int i = 0;
        for (Movie* movie : room->getMovies())
        {
            res->ids[i] = movie->getId();
            i++;
        }

        std::cout << "<- " << res;
        user->sendMessage((struct Message*) res);

        free(res);
    }
}
