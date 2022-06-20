#include "vampire.h"
#include "character.h"
#include "player.h"
#include <memory>

Vampire::Vampire() :  Player{50, 25, 25, "Vampire"} {}

int Vampire::attack(std::shared_ptr<Character> defender)
{
    int dmg = Player::attack(defender);
    // Vampires are allergic to dwarves and lose 5 HP rather than gain)
    if (defender->getName() == "Dwarf")
    {
        hp -= 5;
    }
    // gains 5 HP every successful attack
    else if (dmg != 0)
    {
        hp += 5;
    }
    return dmg;
}
