#include "potion.h"
#include "item.h"
#include <string>
using namespace std;
// Create a dummy potion
Potion::Potion() : 
            Item{"", 'P', Color::BLUE}, deltaHp{0}, deltaAtk{0}, deltaDef{0}, isPermanent{false}, description{""} {}

Potion::Potion(string name, int deltaHp, int deltaAtk, int deltaDef, bool isPermanent, string description) : 
            Item{name, 'P', Color::BLUE}, deltaHp{deltaHp}, deltaAtk{deltaAtk}, deltaDef{deltaDef}, isPermanent{isPermanent}, description{description} {}

int Potion::getDeltaHp() {
    return deltaHp;
}

int Potion::getDeltaAtk() {
    return deltaAtk;
}

int Potion::getDeltaDef() {
    return deltaDef;
}

bool Potion::getPermanent() {
    return isPermanent;
}

string Potion::getDescription() {
    return description;
}
