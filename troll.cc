#include "troll.h"
#include "character.h"
#include "player.h"
#include <algorithm>

using namespace std;

Troll::Troll() :  Player{120, 25, 15, "Troll"} {}


void Troll::abilityPassive()
{
    // regains 5 HP every turn; HP is capped at 120 HP
    hp = min(startingHp, hp+5);
    return;
}
