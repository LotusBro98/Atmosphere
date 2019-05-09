//
// Created by alex on 08.05.19.
//

#ifndef ATMOSPHERE_SERVER_H
#define ATMOSPHERE_SERVER_H


#include <vlc/vlc.h>
#include "Room.h"

class Server {
public:


    void addRoom(Room* room);
    void addMovie(Movie* movie);

    Room* getRoom(int id);
    Movie* getMovie(int id);

    friend std::ostream& operator << (std::ostream& os, Server* server);

    libvlc_instance_t* getVLCinst();

    static Server* getServer()
    {
        static Server s;
        return &s;
    }

private:
    Server();

    libvlc_instance_t* vlc_inst;

    std::vector<Room*> rooms;
    std::vector<Movie*> movies;

    int loadMovies();
    int loadRooms();

};

int main(int args, char * argv[]);


#endif //ATMOSPHERE_SERVER_H
