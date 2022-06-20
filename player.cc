#include "player.h"
#include "character.h"
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include "potion.h"
#include <memory>
using namespace std;

Player::Player(int hp, int atk, int def, string race) : Character{hp, atk, def, '@', race},
                startingHp{hp}, startingAtk{atk}, startingDef{def}, potionMagnifier{1.0}, exp{0}, lvl{0} {}

Player::Player(int hp, int atk, int def, std::string race, float potionMagnifier) : Character{hp, atk, def, '@', race},
                startingHp{hp}, startingAtk{atk}, startingDef{def}, potionMagnifier{potionMagnifier}, exp{0}, lvl{0} {}


void Player::applyPotion(shared_ptr<Potion> potion, bool newLevel)
{
    if (!newLevel)
    {
        // Store the potion
        potionsConsumed.push_back(potion);
    }

    // Apply the potion effect with magnifier (default is 1.0x, 1.5x for drow)
    // Stats cannot drop below zero
    setHP(max(0, (int)((float)hp + (float)(potionMagnifier*potion->getDeltaHp()))));
    setAtk(max(0, (int)((float)atk + (float)(potionMagnifier*potion->getDeltaAtk()))));
    setDef(max(0, (int)((float)def + (float)(potionMagnifier*potion->getDeltaDef()))));

    // Max HP for all races is the starting HP, except for vampires that have no maximum.
    // Cap the health to startingHp if it exceeds it
    if (getName() != "Vampire")
    {
        setHP(min(startingHp, hp));
    }
}

void Player::applyPermanentPotions()
{
    // Reset the starting atk and def
    setAtk(startingAtk);
    setDef(startingDef);
    for (int i = 0; i < potionsConsumed.size(); i++)
    {
        // If it's a temporary potion, remove it
        if (!(potionsConsumed[i]->getPermanent()))
        {
            potionsConsumed[i] = potionsConsumed.back();
            potionsConsumed.pop_back();
        }
        // Otherwise keep the permanent potion effect
        else
        {
            // applyPotion(potionsConsumed[i], true);
        }
    }
}

int Player::attack(std::shared_ptr<Character> defender)
{
    // If attempting to attack a halfing, there is a 50% chance of missing
    if (defender->getName() == "Halfling")
    {
        if (rand() % 2)
        {
            return 0;
        }
    }

    int dmg = 0;
    // Check that the defender is in range
    if (abs(defender->getX() - getX()) <= 1 &&
        abs(defender->getY() - getY()) <= 1)
    {
        dmg = (int)(ceil((100.0 / (100.0 + (float)defender->getDef())) * (float)getAtk()));
    }
    // Decrement the health
    defender->setHP(defender->getHP() - dmg);
    return dmg;
}

// A passive ability is an ability that is applied at every turn
void Player::abilityPassive()
{
    // By default, players have no passive ability
    return;
}


std::string Player::addExp(int expAmt)
{
    std::string result = "";
    exp += expAmt;
    // Check if we have enough exp to level up
    if (exp + 10*lvl >= 10*(lvl+1))
    {
        result = levelUp();
        lvl += 1;
    }
    return result;
}
int Player::getExp()
{
    return exp;
}

int Player::getLvl()
{
    return lvl;
}

// Upgrade a player's attributes after levelling up
std::string Player::levelUp()
{
    // Randomly select an attribute to upgrade
    int roll = (rand() % 3);

    switch (roll)
    {
        // Upgrade hp
        case 0:
        {
            startingHp += 5;
            hp += 5;
            return "HP";
        }
        case 1:
        {
            startingAtk += 5;
            atk += 5;
            return "Atk";
        }
        case 2:
        {
            startingDef += 5;
            def += 5;
            return "Def";
        }
        default:
        {
            cerr << "Error generating attribute to upgrade";
        }
    }
    return "";
}
