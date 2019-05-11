//
// Created by alex on 08.05.19.
//

#ifndef ATMOSPHERE_CLIENT_H
#define ATMOSPHERE_CLIENT_H


#include <vlc/vlc.h>
#include "Server.h"
#include "Player.h"

class Client {
public:

    static Client* getClient()
    {
        static Client c;
        return &c;
    }

    Server* getServer();
    Player* getPlayer();

    void playerMain();

private:
    Client();
    ~Client();

    void connectToServer();

    Server* server;
    Player* player;
};

#endif //ATMOSPHERE_CLIENT_H
