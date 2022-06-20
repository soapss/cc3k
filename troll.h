#ifndef TROLL_H
#define TROLL_H

#include "player.h"

class Troll : public Player {
    public:
        Troll();
        void abilityPassive() override;
};
#endif
