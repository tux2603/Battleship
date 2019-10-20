#ifndef INCLUDE_GUARD_SERVER
#define INCLUDE_GAURD_SERVER

#include <netinet/in.h>
#include "player.h"

#define DEFAULT_PLAYERS 2
#define DEFAULT_PORT 8000
#define BUFFER_SIZE 144

class Server {
    public:
        // ##### Constructors #####
        Server();
        Server(int);
        Server(int, int);

        // ##### Destructors #####
        ~Server();

        // ##### Getters and Setters #####
        int getNumPlayers();
        int getPort();

        bool handleNextTurn();

    private:
        const int addrlen = sizeof(address);
        struct sockaddr_in address;
        int numPlayers,
            currentPlayer,
            port,
            socketFD,
            opt=1;

        Player **players;
};

#endif
