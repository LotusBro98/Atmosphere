//
// Created by alex on 08.05.19.
//

#include "include/Room.h"
#include "include/Server.h"
#include <iostream>

Room::Room(int id) {
    this->id = id;
}

void Room::addMovie(Movie *movie) {
    movies.push_back(movie);
}

inline int Room::getId() { return id; }

std::ostream &operator<<(std::ostream &os, Room *room) {
    os << "Room " << room->getId() << ":\n";
    for (Movie* movie : room->movies){
        os << "\t" << movie << "\n";
    }
    return os;
}

void Room::startStreaming(Movie *movie) {
    libvlc_instance_t* vlc_inst = Server::getServer()->getVLCinst();
    const char * media_name = ("room/" + std::to_string(id)).data();
    char * source = movie->getSource();

    const char *sout_fmt = "#standard{access=http,mux=ts,dst=:8080/%s}";
    char sout[256];
    sprintf(sout, sout_fmt, media_name);

    libvlc_vlm_add_broadcast(vlc_inst, media_name, source, sout, 0, NULL, true, true);
    libvlc_vlm_play_media(vlc_inst, media_name);
}

void Room::startStreaming(int index) {
    Movie* movie = movies.at(index);
    startStreaming(movie);
}

void Room::pause() {
    libvlc_instance_t* vlc_inst = Server::getServer()->getVLCinst();
    const char * media_name = ("room/" + std::to_string(id)).data();

    libvlc_vlm_pause_media(vlc_inst, media_name);
}

void Room::resume() {
    libvlc_instance_t* vlc_inst = Server::getServer()->getVLCinst();
    const char * media_name = ("room/" + std::to_string(id)).data();

    libvlc_vlm_play_media(vlc_inst, media_name);
}
