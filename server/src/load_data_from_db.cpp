//
// Created by alex on 08.05.19.
//

#include <sqlite3.h>
#include <cstring>
#include "../include/Server.h"

using namespace server;

const char * statement_movies_table = "create table movies(\n"
                                      "id integer not null constraint movies_pk primary key autoincrement,\n"
                                      "source text not null\n"
                                      ");";
const char * statement_get_movies = "SELECT id, source FROM movies;";

int movie_row_callback(void *serverPtr, int cols, char **data, char **azColName) {
    int id = atoi(data[0]);
    char* source = data[1];

    Movie* movie = new Movie(id, source);

    ((Server*) serverPtr)->addMovie(movie);

    return 0;
}

int Server::loadMovies() {
    sqlite3* db;
    char* errMsg = 0;

    if (sqlite3_open("serverDB.sqlite", &db))
    {
        fprintf(stderr, "Can't open server database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    if (sqlite3_exec(db, statement_get_movies, movie_row_callback, this, &errMsg)) {
        fprintf(stderr, "Cannot load movies: %s\n", errMsg);
        fprintf(stderr, "Creating new table 'movies'");
        if (sqlite3_exec(db, statement_movies_table, 0, 0, &errMsg))
        {
            fprintf(stderr, "Failed to load movies: %s\n", errMsg);
            sqlite3_free(errMsg);
            return 1;
        }
    }
    sqlite3_close(db);
    return 0;
}

void Server::addMovie(Movie *movie) {
    movies.push_back(movie);
}

void Server::addRoom(Room *room) {
    rooms.push_back(room);
}


const char * statement_rooms_table = "create table rooms\n"
                                     "(\n"
                                     "\tid integer not null\n"
                                     "\t\tconstraint rooms_pk\n"
                                     "\t\t\tprimary key autoincrement\n"
                                     ");\n"
                                     "";
const char * statement_get_rooms = "SELECT id FROM rooms;";

int room_row_callback(void *serverPtr, int cols, char **data, char **azColName) {
    int id = atoi(data[0]);

    Room* room = new Room(id);

    ((Server*) serverPtr)->addRoom(room);

    return 0;
}

const char * statement_movies_in_rooms_table = "create table movies_in_rooms\n"
                                               "(\n"
                                               "\troom integer not null,\n"
                                               "\tmovie integer not null\n"
                                               ");\n"
                                               "";
const char * statement_get_movies_in_rooms = "SELECT room, movie FROM movies_in_rooms;";

int fill_room_row_callback(void *serverPtr, int cols, char **data, char **azColName) {
    int room_id = atoi(data[0]);
    int movie_id = atoi(data[1]);

    Server* server = ((Server*) serverPtr);

    Room* room = server->getRoom(room_id);
    Movie* movie = server->getMovie(movie_id);

    if (room == nullptr || movie == nullptr)
    {
        fprintf(stderr, "Error while trying to add movie to room. Some ids are wrong. Room: %d, Movie %d", room_id, movie_id);
        return 0;
    }

    room->addMovie(movie);

    return 0;
}

int Server::loadRooms() {
    sqlite3* db;
    char* errMsg = 0;

    if (sqlite3_open("serverDB.sqlite", &db))
    {
        fprintf(stderr, "Can't open server database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    if (sqlite3_exec(db, statement_get_rooms, room_row_callback, this, &errMsg)) {
        fprintf(stderr, "Cannot load rooms: %s\n", errMsg);
        fprintf(stderr, "Creating new table 'rooms'\n");
        if (sqlite3_exec(db, statement_rooms_table, 0, 0, &errMsg))
        {
            fprintf(stderr, "Failed to load rooms: %s\n", errMsg);
            sqlite3_free(errMsg);
            return 1;
        }
    }

    if (sqlite3_exec(db, statement_get_movies_in_rooms, fill_room_row_callback, this, &errMsg)) {
        fprintf(stderr, "Cannot fill rooms with movies: %s\n", errMsg);
        fprintf(stderr, "Creating new table 'movies_in_rooms'\n");
        if (sqlite3_exec(db, statement_movies_in_rooms_table, 0, 0, &errMsg))
        {
            fprintf(stderr, "Failed to fill rooms with movies: %s\n", errMsg);
            sqlite3_free(errMsg);
            return 1;
        }
    }
    sqlite3_close(db);
    return 0;
}