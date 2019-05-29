//
// Created by alex on 08.05.19.
//

#include <iostream>
#include <cstring>
#include "../include/Movie.h"
#include "../../client/include/Movie.h"


namespace server {
    Movie::Movie(int id, char* source) {
        this->source = strdup(source);
        this->id = id;
    }

    std::ostream &operator<<(std::ostream &os, Movie *movie) {
        std::string source(movie->source);
        os << movie->id << ": " << source;
        return os;
    }

    char* Movie::getSource() {
        return source;
    }

}

