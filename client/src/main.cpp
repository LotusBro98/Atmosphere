//
// Created by alex on 10.05.19.
//

#include <Client.h>
#include <unistd.h>

int main()
{
    Client* client = Client::getClient();

    Server* server = client->getServer(0);

    if (!server->isConnected())
    {
        fprintf(stderr, "Failed to connect to server. Exiting");
        return 1;
    }

    new Room(1, server);
    Room* room = server->getRoom(1);

    room->pause();

    sleep(10);
}