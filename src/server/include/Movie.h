//
// Created by alex on 08.05.19.
//

#ifndef ATMOSPHERESERVER_MOVIE_H
#define ATMOSPHERESERVER_MOVIE_H


#include <string>

class Movie {
public:
    Movie(int id, char* source);

    friend std::ostream& operator << (std::ostream& os, Movie* movie);

    inline int getId() { return id; };

    char* getSource();

private:
    int id;
    char* source;
};


#endif //ATMOSPHERESERVER_MOVIE_H
