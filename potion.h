#ifndef POTION_H
#define POTION_H
#include "item.h"
#include <string>

class Potion : public Item {
    int deltaHp;
    int deltaAtk;
    int deltaDef;
    bool isPermanent;
    std::string description;
    public:
    Potion();
    Potion(std::string name, int deltaHp, int deltaAtk, int deltaDef, bool isPermanent, std::string description);
    int getDeltaHp();
    int getDeltaAtk();
    int getDeltaDef();
    bool getPermanent();
    std::string getDescription();

    static const int NUM_POTION_TYPES = 6;

    enum PotionTypes {
        RESTOREHEALTH = 1,
        POISONHEALTH,
        BOOSTATK,
        WOUNDATK,
        BOOSTDEF,
        WOUNDDEF
    };

};

#endif
