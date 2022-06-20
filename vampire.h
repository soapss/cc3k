#ifndef VAMPIRE_H
#define VAMPIRE_H

#include "player.h"

class Vampire : public Player {
    public:
        Vampire();
        int attack(std::shared_ptr<Character> defender) override;
};
#endif
