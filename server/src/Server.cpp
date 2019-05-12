//
// Created by alex on 08.05.19.
//

#include "../include/Server.h"

#include <sqlite3.h>
#include <iostream>
#include <unistd.h>
#include <poll.h>

using namespace server;

Server::Server() {
    this->vlc_inst = libvlc_new(0, NULL);
    if (vlc_inst == NULL)
    {
        fprintf(stderr, "Failed to load libVNC. Exiting.");
        exit(1);
    }
    loadMovies();
    loadRooms();
}

Room *Server::getRoom(int id) {
    for (Room* room : rooms)
    {
        if (room->getId() == id)
            return room;
    }
    return nullptr;
}

Movie *Server::getMovie(int id) {
    for (Movie* movie : movies)
    {
        if (movie->getId() == id)
            return movie;
    }
    return nullptr;
}

std::ostream& server::operator<<(std::ostream &os, Server *server) {
    os << "Rooms: \n";
    for (Room *room : server->rooms) {
        os << room;
    }
    os << "\nMovies: \n";
    for (Movie *movie : server->movies) {
        os << "\t" << movie << "\n";
    }
    return os;
}

libvlc_instance_t *Server::getVLCinst() {
    return vlc_inst;
}

bool Server::isAlive() {
    return true;
}

void Server::addUser(User *user) {
    users.push_back(user);
}

void Server::removeUserFromEverywhere(User *user) {
    //users.remove(user);
    close(user->getSockFD());
    //printf("Closed %d\n", user->getSockFD());
    errno = 0;

    for (Room* room: rooms)
    {
        room->removeUser(user);
    }
}

std::list<User *>& Server::getUsers() {
    return users;
}

std::list<Room *>& Server::getRooms() {
    return rooms;
}
