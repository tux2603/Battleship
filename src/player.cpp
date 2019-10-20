#include <iostream>
#include "player.h"

using namespace std;

Player::Player(int sessionFD, string name) : session(sessionFD), name(name) {
    cout << "Created player " << name << " (session " << sessionFD << ")" << endl;
}

Player::Player(int sessionFD) : Player::Player(sessionFD, "Player " + to_string(sessionFD)) {}

string Player::getName() {
    return name;
}

int Player::getSession() {
    return session;
}
