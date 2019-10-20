#ifndef INCLUDE_GAURD_PLAYER
#define INCLUDE_GAURD_PLAYER

#include <string>

using namespace std;

class Player {
    public:
        // ##### Constructors #####

        Player(int);
        Player(int, string);

        // ##### Destructors #####
        ~Player();

        // ##### Getters and Setters #####
        int getSession();
        string getName();

    private:
        int session;
        string name;
};

#endif
