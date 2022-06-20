#ifndef DRAGON_H
#define DRAGON_H
#include "enemy.h"
#include "dragonHoard.h"
#include <memory>
class Dragon : public Enemy {
    std::shared_ptr<DragonHoard> theHoard;
    public:
    Dragon(std::shared_ptr<DragonHoard> theHoard);
    int attack(std::shared_ptr<Character> defender) override;
    void move(int dx, int dy) override;
    bool inRange(std::shared_ptr<Character> defender) override;
    std::shared_ptr<DragonHoard> getHoard();
    void setHoard(std::shared_ptr<DragonHoard> dragonHoard);
};
#endif
