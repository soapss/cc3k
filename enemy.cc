#include "enemy.h"
#include "character.h"
#include <string>
#include <memory>
#include <cmath>

using namespace std;

Enemy::Enemy(int startingHp, int startingAtk, int startingDef, bool hostile, char symbol, std::string name) :
    Character{startingHp, startingAtk, startingDef, symbol, name, Color::RED}, hostile{hostile}, speed{1} {}

int Enemy::attack(shared_ptr<Character> defender) {     
    int dmg = 0;
    // Check that the defender is in range
    if (abs(defender->getX() - getX()) <= 1 &&
        abs(defender->getY() - getY()) <= 1)
    {
        // 50% chance of missing
        if (rand() % 2 )
        {
            return 0;
        }
        dmg = (int)(ceil((100.0/(100.0+defender->getDef())*getAtk())));
    }
    // Decrement the health
    defender->setHP(defender->getHP()-dmg);
    return dmg;
}

bool Enemy::inRange(shared_ptr<Character> defender) {
    return (abs(defender->getX() - getX()) <= 1 &&
        abs(defender->getY() - getY()) <= 1);
}

void Enemy::move(int dx, int dy) {
    // Discard dx and dy
    // Generate a random -1, 0, 1
    Character::move(dx,dy);
}
