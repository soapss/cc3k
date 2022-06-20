#ifndef PATHFINDER_H
#define PATHFINDER_H
#include "enemy.h"
#include <memory>
#include "cc3k.h"
class Pathfinder : public Enemy {
    public:
    Pathfinder();
    // void move(int dx, int dy) override;
    static std::pair<int, int> bfs(CC3K &game, int enemyX, int enemyY);
    void move(int dx, int dy) override;
};
#endif
