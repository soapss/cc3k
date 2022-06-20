#ifndef ORC_H
#define ORC_H
#include "enemy.h"
#include <memory>
class Orc : public Enemy {
    public:
    Orc();
    int attack(std::shared_ptr<Character> defender) override;
    // void move(int dx, int dy) override;
};
#endif
