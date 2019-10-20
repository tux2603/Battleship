#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server.h"
#include "player.h"

using namespace std;

Server::Server(int numPlayers, int port) : numPlayers(DEFAULT_PLAYERS), port(port) {
    cout << "Started a server on port " << port << " with " << numPlayers << " players" << endl;
    players = new Player*[numPlayers];
    currentPlayer = 0;

    // ##### Begin magic #####

    // Try to create a file descriptor for the socket
    if ( (socketFD = socket(AF_INET, SOCK_STREAM, 0)) == 0 )
        throw "Could not create socket file descriptor";

    if(setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        throw "Could not set up socket options";

    // Set some parameters for the address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Not strictly necessary, but keeps the vscode syntax coloring happy
    struct sockaddr *addrPointer =  (struct sockaddr*)&address;

    // Bind the socket
    if (bind(socketFD, addrPointer, sizeof(address)) < 0) 
        throw "Could not bind socket to port " + to_string(port);

    // Start listening on the socket
    if (listen(socketFD, 256))
        throw "Could not listen on socket";

    // ##### Magic is over #####

    for(int i = 0; i < numPlayers; ++i) {
        cout << "Waiting for player " << i+1 << " to connect" << endl;

        int sessionFD = accept(socketFD, (struct sockaddr *)&address, (socklen_t *)&addrlen);

        if(sessionFD >= 0) {
            Player *newPlayer = new Player(sessionFD);
            players[i] = newPlayer;

            // Send a message to the player
            string message = "Hello, you are player " + to_string(i+1) + "\n";
            send(sessionFD, message.c_str(), message.length(), 0);
        }
    }
}

Server::Server(int numPlayers) : Server::Server(DEFAULT_PLAYERS, DEFAULT_PORT) {}
Server::Server() : Server::Server(DEFAULT_PLAYERS, DEFAULT_PORT) {}

Server::~Server() {
    cout << "Server on port " << port << " destroyed" << endl;
}

int Server::getNumPlayers() {
    return numPlayers;
}

int Server::getPort() {
    return port;
}

bool Server::handleNextTurn() {

    cout << "Waiting for something from player" << currentPlayer+1 << endl;

    // Send a message to the player that we want input
    string message = "Player " + to_string(currentPlayer+1) + ", please say something\n";
    send(players[currentPlayer]->getSession(), message.c_str(), message.length(), 0);
    
    // Create a buffer to hold the string read in
    char buffer[BUFFER_SIZE] = {0};

    // Read into that buffer
    int charsRead = read(players[currentPlayer]->getSession(), buffer, BUFFER_SIZE);

    // Terminate the string
    if (charsRead < BUFFER_SIZE)
        buffer[charsRead] = 0;
    else
        buffer[BUFFER_SIZE - 1] = 0;

    string userString = string(buffer);
    message = "Player " + to_string(currentPlayer+1) + " said " + userString;

    // Relay the message to all players
    for(int i = 0; i < numPlayers; ++i) {
        send(players[i]->getSession(), message.c_str(), message.length(), 0);
    }

    // Increment whoever turn it is
    ++currentPlayer;
    currentPlayer %= numPlayers;

    return true;
}
