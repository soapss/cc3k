#ifndef LEVEL_H_
#define LEVEL_H_

#include <memory>

class Floor;
class Player;
class Stairway;
class Potion;
class Gold;
class Enemy;
class Dragon;

class Level {
protected:
    std::shared_ptr<Floor> theFloor;
public:
	Level(std::shared_ptr<Floor> floorMap);

    virtual std::shared_ptr<Player> generatePlayer(int startingRace) = 0;
    virtual std::shared_ptr<Stairway> generateStairway(int playerChamberNum) = 0;
    virtual std::shared_ptr<Potion> generatePotion() = 0;
    virtual std::shared_ptr<Gold> generateGold(std::shared_ptr<Dragon> dragonEnemy) = 0;
    virtual std::shared_ptr<Enemy> generateEnemy(bool hostileMerchants) = 0;
    virtual void placePlayer(std::shared_ptr<Player> thePlayer) = 0;

    // Helper functions for parsing custom levels
    virtual std::shared_ptr<Potion> spawnPotionAt(int potionType, int x, int y) = 0;
    virtual std::shared_ptr<Gold> spawnGoldAt(int goldType, int x, int y, std::shared_ptr<Dragon> dragon) = 0;

	virtual ~Level() = 0;


};

#endif /* LEVEL_H_ */
