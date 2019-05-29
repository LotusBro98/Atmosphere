//
// Created by alex on 08.05.19.
//

#include "../include/Room.h"
#include <iostream>
#include <cstring>
#include <sys/time.h>

using namespace server;

Room::Room(int id) {
    this->id = id;
    this->playing = false;
}

void Room::addMovie(Movie *movie) {
    movies.push_back(movie);
}

std::ostream& server::operator<<(std::ostream &os, Room *room) {
    os << "Room " << room->getId() << ":\n";
    for (Movie *movie : room->movies) {
        os << "\t" << movie << "\n";
    }
    return os;
}

/*
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
 */

void Room::pause() {
    MsgPause msg;
    msg.room = this->id;

    rememberSeek();
    playing = false;
    printf("Room %d. Paused\n", id);

    for (User* user : users)
    {
        user->sendMessage((struct Message*) &msg);
    }
}

void Room::resume() {
    MsgResume msg;
    msg.room = this->id;

    rememberSeek();
    playing = true;
    printf("Room %d. Resumed\n", id);

    for (User* user : users)
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

    free(msg);
}

time_t time_msec()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void Room::selectMovie(int index) {
    this->currentMovie = movies.back();
    lastSeekTime = time_msec();
    lastSeek = -1;
    playing = false;
}

void Room::seek(float progress) {
    MsgSeek msg;
    msg.room = this->id;
    msg.percentage = progress;

    lastSeekTime = time_msec();
    lastSeek = (int) progress;

    for (User* user : users)
    {
        user->sendMessage((struct Message*) &msg);
    }
}

float Room::askSeek()
{
    if (playing)
    {
        return time_msec() - lastSeekTime + lastSeek;
    } else {
        return lastSeek;
    }
}

void Room::addUser(User *user) {
    users.push_back(user);

    //seek(askSeek());
}

void Room::removeUser(User *user) {
    users.remove(user);

    printf("Room %d, users left: %d\n", this->getId(), users.size());

    if (users.size() == 0)
        this->pause();
}

bool Room::isPlaying() {
    return playing;
}

void Room::rememberSeek() {
    lastSeek = askSeek();
    lastSeekTime = time_msec();

    printf("Remembered last seek: %ld\n", lastSeek);
}

std::list<Movie *> Room::getMovies() {
    return this->movies;
}
