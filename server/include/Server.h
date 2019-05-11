//
// Created by alex on 08.05.19.
//

#ifndef ATMOSPHERE_SERVER_H
#define ATMOSPHERE_SERVER_H


#include <vlc/vlc.h>
#include "Room.h"
#include <list>

namespace server {

    class Server {
    public:


        void addRoom(Room *room);

        void addMovie(Movie *movie);

        void addUser(User *user);
        void removeUser(User *user);

        Room *getRoom(int id);

        Movie *getMovie(int id);

        friend std::ostream &operator<<(std::ostream &os, Server *server);

        libvlc_instance_t *getVLCinst();

        static Server *getServer() {
            static Server s;
            return &s;
        }

        bool isAlive();

        int openSocket(short port = 23443);

        std::list<User *>& getUsers();
        std::list<Room *>& getRooms();

    private:
        Server();

        pthread_t listener_thread;

        libvlc_instance_t *vlc_inst;

        std::list<Room *> rooms;
        std::list<Movie *> movies;
        std::list<User *> users;

        int loadMovies();
        int loadRooms();
    };

    int main(int args, char *argv[]);

}


#endif //ATMOSPHERE_SERVER_H
