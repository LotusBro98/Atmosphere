//
// Created by alex on 08.05.19.
//

#include "../include/Room.h"
#include "../include/Server.h"
#include "../../include/Message.h"
#include <iostream>
#include <cstring>

using namespace server;

Room::Room(int id) {
    this->id = id;
}

void Room::addMovie(Movie *movie) {
    movies.push_back(movie);
}

inline int Room::getId() { return id; }

std::ostream& server::operator<<(std::ostream &os, Room *room) {
    os << "Room " << room->getId() << ":\n";
    for (Movie *movie : room->movies) {
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
    MsgPause msg;
    msg.room = this->id;

    for (User* user : Server::getServer()->getUSers())
    {
        user->sendMessage((struct Message*) &msg);
    }
}

void Room::resume() {
    MsgResume msg;
    msg.room = this->id;

    for (User* user : Server::getServer()->getUSers())
    {
        user->sendMessage((struct Message*) &msg);
    }
}

void Room::sendSource(User *user) {
    char* source = this->currentMovie->getSource();
    int size = strlen(source) + 1;
    size += (long)(((struct MsgSource*)0)->source);

    struct MsgSource* msg = (struct MsgSource*) malloc(size);

    msg->type = MSG_SOURCE;
    msg->size = size;
    msg->room = this->id;
    strcpy(msg->source, source);

    user->sendMessage((struct Message*) msg);
}

void Room::selectMovie(int index) {
    this->currentMovie = movies.at(index);
}
