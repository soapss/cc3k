#include "dragonslayer.h"
#include "player.h"
#include "character.h"

#include <iostream>
#include <cmath>
using namespace std;

Dragonslayer::Dragonslayer() :  Player{200, 50, 50, "Dragonslayer"} {}

int Dragonslayer::attack(std::shared_ptr<Character> defender)
{
    int dmg;
    if (defender->getName() == "Dragon")
    {
        hp += 5;
        atk += 10000;
        dmg = Player::attack(defender);
        atk -= 10000;
    }
    else
    {
        dmg = Player::attack(defender);
    }

    return dmg;
}
