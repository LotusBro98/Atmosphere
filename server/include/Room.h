//
// Created by alex on 08.05.19.
//

#ifndef ATMOSPHERESERVER_ROOM_H
#define ATMOSPHERESERVER_ROOM_H


#include <vector>
#include "Movie.h"
#include "User.h"

#include <vlc/vlc.h>

namespace server {

    class Room {
    public:

        Room(int id);

        void addMovie(Movie *movie);

        void selectMovie(int index);

        void startStreaming(Movie *movie);

        void startStreaming(int index = 0);

        void pause();

        void resume();

        inline int getId();

        friend std::ostream &operator<<(std::ostream &os, Room *room);

        void sendSource(User* user);

    private:

        std::vector<Movie *> movies;
        std::vector<User *> users;

        Movie *currentMovie = nullptr;

        int id;
    };

}

#endif //ATMOSPHERESERVER_ROOM_H
