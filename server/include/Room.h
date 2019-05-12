//
// Created by alex on 08.05.19.
//

#ifndef ATMOSPHERESERVER_ROOM_H
#define ATMOSPHERESERVER_ROOM_H

namespace server {
    class Room;
}

#include <vector>
#include "Movie.h"
#include "User.h"

#include <list>

#include <vlc/vlc.h>

namespace server {

    class Room {
    public:

        Room(int id);

        void addMovie(Movie *movie);

        void addUser(User* user);
        void removeUser(User* user);

        void selectMovie(int index);

        //void startStreaming(Movie *movie);
        //void startStreaming(int index = 0);

        void pause();
        void resume();
        void seek(float progress);

        float askSeek();
        void rememberSeek();

        bool isPlaying();

        inline int getId() { return id;}

        friend std::ostream &operator<<(std::ostream &os, Room *room);

        void sendSource(User* user);

    private:

        std::list<Movie *> movies;
        std::list<User *> users;

        Movie *currentMovie = NULL;

        time_t lastSeekTime;
        time_t lastSeek;
        bool playing;

        int id;
    };

}

#endif //ATMOSPHERESERVER_ROOM_H
