#include "server.h"

int main() {
    Server myServer = Server(2, 5000);
    while(myServer.handleNextTurn());
    return 0;
}
