//
// Created by alex on 08.05.19.
//

#ifndef ATMOSPHERE_CLIENT_H
#define ATMOSPHERE_CLIENT_H


#include <vlc/vlc.h>
#include "Server.h"

class Client {
public:

    static Client* getClient()
    {
        static Client c;
        return &c;
    }

    void addServer(Server* server);

    Server* getServer(int index);

private:
    Client();

    void loadServers();

    std::vector<Server*> knownServers;

    Server* currentServer;
    Room* currentRoom;

    libvlc_instance_t* vlc_inst;
    libvlc_media_player_t* main_player;
};

#endif //ATMOSPHERE_CLIENT_H
