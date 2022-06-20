#ifndef DRAGONSLAYER_H
#define DRAGONSLAYER_H

#include "player.h"

class Dragonslayer : public Player {
    public:
        Dragonslayer();
        int attack(std::shared_ptr<Character> defender) override;
};
#endif
