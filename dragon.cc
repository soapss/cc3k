#include "dragon.h"
#include "enemy.h"
#include <iostream>
#include <cmath>
using namespace std;

Dragon::Dragon(std::shared_ptr<DragonHoard> theHoard) : Enemy{150, 20, 20, false, 'D', "Dragon"}, theHoard{theHoard} {}


void Dragon::move(int dx, int dy)
{
    // Dragon is stationary
    return;
}

int Dragon::attack(std::shared_ptr<Character> defender)
{
    int dmg = 0;
    // Check that the defender is in range (NOTE: this includes the gold hoard)
    if (inRange(defender))
    {
        // 50% chance of missing
        if (rand() % 2)
        {
            return 0;
        }
        dmg = (int)(ceil((100.0 / (100.0 + defender->getDef())) * getAtk()));
    }
    // Decrement the health
    defender->setHP(defender->getHP() - dmg);
    return dmg;
}

std::shared_ptr<DragonHoard> Dragon::getHoard()
{
    return theHoard;
}

void Dragon::setHoard(std::shared_ptr<DragonHoard> dragonHoard) 
{
    theHoard = dragonHoard;
}



bool Dragon::inRange(std::shared_ptr<Character> defender)
{
    /*
        Dragons are considered hostile when the player is next to (read: in the 1 block radius of) its
        dragon hoard or itself, and will use their fire breath to defend its hoard (i.e. will attack the enemy). This means that a Dragon
        might attack even if the player is not next to the Dragon, but because it is next to a dragon hoard.
    */
    bool inRangeOfItself = Enemy::inRange(defender);
    bool inRangeOfHoard = (abs(defender->getX() - theHoard->getX()) <= 1 
                        && abs(defender->getY() - theHoard->getY()) <= 1);

    return inRangeOfItself || inRangeOfHoard;

}

