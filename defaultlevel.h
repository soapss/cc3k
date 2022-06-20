#ifndef DEFAULTLEVEL_H_
#define DEFAULTLEVEL_H_

#include <memory>

#include "level.h"

class CC3K;
class Floor;
class Player;
class Stairway;
class Potion;
class Gold;
class Enemy;
class Dragon;

class DefaultLevel: public Level {
	const CC3K &game;

public:
	DefaultLevel(std::shared_ptr<Floor> floorMap, const CC3K &game);

    std::shared_ptr<Player> generatePlayer(int startingRace) override;
    std::shared_ptr<Stairway> generateStairway(int playerChamberNum) override;
    std::shared_ptr<Potion> generatePotion() override;
    std::shared_ptr<Gold> generateGold(std::shared_ptr<Dragon> dragonEnemy) override;
    std::shared_ptr<Enemy> generateEnemy(bool hostileMerchants) override;
    void placePlayer(std::shared_ptr<Player> thePlayer) override;

    // Helper functions for parsing custom levels
    std::shared_ptr<Potion> spawnPotionAt(int potionType, int x, int y);
    std::shared_ptr<Gold> spawnGoldAt(int goldType, int x, int y, std::shared_ptr<Dragon> dragon);

};

#endif /* DEFAULTLEVEL_H_ */
