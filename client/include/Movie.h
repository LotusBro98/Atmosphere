//
// Created by amburkoff on 12.05.19.
//

#ifndef PLAYER_TEST_MOVIE_H
#define PLAYER_TEST_MOVIE_H


#include <string>

class Movie {
public:
    Movie(int id);

private:
    int id;
    std::string url;
};


#endif //PLAYER_TEST_MOVIE_H
