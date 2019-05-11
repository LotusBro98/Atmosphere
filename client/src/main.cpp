//
// Created by alex on 10.05.19.
//

#include <Client.h>
#include <unistd.h>
#include <Player.h>

int main(int argc, char* argv[])
{
    errno = 0;
    Client* client = Client::getClient();

    Server* server = client->getServer();
    if (!server->isConnected())
    {
        fprintf(stderr, "Failed to connect to server. Exiting");
        return 1;
    }

    new Room(1, server);
    Room* room = server->getRoom(1);

    client->playerMain();
    server->disconnect();
}