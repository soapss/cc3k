#include "defaultlevel.h"

#include "cc3k.h"

#include <iostream>

#include "player.h"
#include "drow.h"
#include "shade.h"
#include "goblin.h"
#include "troll.h"
#include "vampire.h"
#include "dragonslayer.h"

#include "enemy.h"
#include "orc.h"
#include "human.h"
#include "dragon.h"
#include "dwarf.h"
#include "halfling.h"
#include "elf.h"
#include "merchant.h"
#include "pathfinder.h"

#include "restoreHealth.h"
#include "poisonHealth.h"
#include "boostAtk.h"
#include "woundAtk.h"
#include "boostDef.h"
#include "woundDef.h"

#include "gold.h"
#include "smallPile.h"
#include "normalPile.h"
#include "dragonHoard.h"
#include "merchantHoard.h"

#include "stairway.h"
#include "floor.h"

using namespace std;

DefaultLevel::DefaultLevel(shared_ptr<Floor> floorMap, const CC3K &game)
	: Level{floorMap}, game{game} {}

/*
 * Generates a player according to startingRace.
 */
shared_ptr<Player> DefaultLevel::generatePlayer(int startingRace)
{
	shared_ptr<Player> newPlayer;
	switch (startingRace)
	{
	case (Player::RaceTypes::SHADE):
	{
		newPlayer = make_shared<Shade>();
		break;
	}
	case (Player::RaceTypes::DROW):
	{
		newPlayer = make_shared<Drow>();
		break;
	}
	case (Player::RaceTypes::VAMPIRE):
	{
		newPlayer = make_shared<Vampire>();
		break;
	}
	case (Player::RaceTypes::GOBLIN):
	{
		newPlayer = make_shared<Goblin>();
		break;
	}
	case (Player::RaceTypes::TROLL):
	{
		newPlayer = make_shared<Troll>();
		break;
	}
	case (Player::RaceTypes::DRAGONSLAYER):
	{
		newPlayer = make_shared<Dragonslayer>();
		break;
	}
	}
	placePlayer(newPlayer);

	return newPlayer;
}

shared_ptr<Stairway> DefaultLevel::generateStairway(int playerChamberNum)
{
	// Assumption: only the player has been generated at this point
	// Thus we only need to check collision with player
	shared_ptr<Stairway> newStairway = make_shared<Stairway>();

	while (true)
	{
		// Generate a random chamber number (to ensure each chamber has equal probability)
		// Ensure it is NOT EQUAL to player chamber
		int targetChamberNum = theFloor->getRandomChamberNum();

		while (targetChamberNum == playerChamberNum)
		{
			targetChamberNum = theFloor->getRandomChamberNum();
		}

		// Get random coordinates and a random chamber number
		int randX = theFloor->getRandomX();
		int randY = theFloor->getRandomY();
		int chamberNum = theFloor->chamberAt(randX, randY);

		// Check that chamber is not same as the chamber that player spawned in
		if (!game.isOccupied(randX, randY) && (chamberNum == targetChamberNum) && (chamberNum != playerChamberNum))
		{
			newStairway->setX(randX);
			newStairway->setY(randY);
			return newStairway;
		}
	}
}

shared_ptr<Potion> DefaultLevel::generatePotion()
{
	// Generate a random potion type
	int potionType = rand() % Potion::NUM_POTION_TYPES + 1;

	auto newPotion = make_shared<Potion>();

	switch (potionType)
	{
	case Potion::PotionTypes::RESTOREHEALTH:
	{
		newPotion = make_shared<RestoreHealth>();
		break;
	}
	case Potion::PotionTypes::POISONHEALTH:
	{
		newPotion = make_shared<PoisonHealth>();
		break;
	}
	case Potion::PotionTypes::BOOSTATK:
	{
		newPotion = make_shared<BoostAtk>();
		break;
	}
	case Potion::PotionTypes::WOUNDATK:
	{
		newPotion = make_shared<WoundAtk>();
		break;
	}
	case Potion::PotionTypes::BOOSTDEF:
	{
		newPotion = make_shared<BoostDef>();
		break;
	}
	case Potion::PotionTypes::WOUNDDEF:
	{
		newPotion = make_shared<WoundDef>();
		break;
	}
	}

	while (true)
	{
		// Generate a random chamber number (to ensure each chamber has equal probability)
		int targetChamberNum = theFloor->getRandomChamberNum();

		// Get random coordinates and its associated chamber
		int randX = theFloor->getRandomX();
		int randY = theFloor->getRandomY();
		int chamberNum = theFloor->chamberAt(randX, randY);

		if ((chamberNum == targetChamberNum) && !(game.isOccupied(randX, randY)))
		{
			newPotion->setX(randX);
			newPotion->setY(randY);
			return newPotion;
		}
	}
	return nullptr;
}

shared_ptr<Gold> DefaultLevel::generateGold(shared_ptr<Dragon> dragonEnemy)
{
	// Generate a number from 1-8
	// For the desired probability distribution
	// 5/8 normal, 1/8 dragon, 1/4 small hoard
	int goldType = rand() % 8 + 1;

	auto newGold = make_shared<Gold>("", 0);

	// If we are spawning a dragonhoard
	bool isDragonHoard = false;

	// 5/8 probability normal
	if (goldType <= 5)
	{
		newGold = make_shared<NormalPile>();
	}
	// 1/8 probability dragon
	else if (goldType <= 6)
	{
		// Spawn a dragon later
		isDragonHoard = true;
	}
	// 1/4 probability small
	else
	{
		newGold = make_shared<SmallPile>();
	}

	while (true)
	{
		// Generate a random chamber number (to ensure each chamber has equal probability)
		int targetChamberNum = theFloor->getRandomChamberNum();

		// Get random coordinates and its associated chamber
		int randX = theFloor->getRandomX();
		int randY = theFloor->getRandomY();
		int chamberNum = theFloor->chamberAt(randX, randY);
		if (chamberNum == targetChamberNum && !(game.isOccupied(randX, randY)))
		{
			if (isDragonHoard)
			{
				// Generate random coordinates for the dragon
				auto dragonGold = make_shared<DragonHoard>();

				// Generate a dragon with the associated dragon hoard
				auto dragon = make_shared<Dragon>(dragonGold);

				// Generate the coordinates for the dragon

				// Generate a dx and dy either -1 or 1
				int dY = rand() % 2 ? -1 : 1;
				int dX = rand() % 2 ? -1 : 1;

				int dragonX = dX + randX;
				int dragonY = dY + randY;

				// Set the coordinates for gold and dragon
				dragonGold->setX(randX);
				dragonGold->setY(randY);

				int dragonChamberNum = theFloor->chamberAt(dragonX, dragonY);

				if (dragonChamberNum == targetChamberNum && !(game.isOccupied(dragonX, dragonY)))
				{
					dragonEnemy->setX(dragonX);
					dragonEnemy->setY(dragonY);
					dragonEnemy->setHoard(dragonGold);

					return dragonGold;
				}
			}
			else
			{
				newGold->setX(randX);
				newGold->setY(randY);
				dragonEnemy->setHoard(nullptr);
				return newGold;
			}
			// If generation fails, we do nothing
		}
	}
	return nullptr;
}

shared_ptr<Enemy> DefaultLevel::generateEnemy(bool hostileMerchants)
{
	// Generate a number from 1-18
	// For the desired probability distribution
	int enemyType = rand() % 18 + 1;

	auto newEnemy = make_shared<Enemy>(0, 0, 0, false, 'E', "");

	if (game.getDLC())
	{
		// DLC spawning probabilities (to accomodate extra enemies)
		// Cumulative probability
		if (enemyType <= 4)
		{
			newEnemy = make_shared<Pathfinder>();
		}
		else if (enemyType <= 6)
		{
			newEnemy = make_shared<Human>();
		}
		//
		else if (enemyType <= 8)
		{
			newEnemy = make_shared<Dwarf>();
		}
		else if (enemyType <= 10)
		{
			newEnemy = make_shared<Halfling>();
		}
		else if (enemyType <= 12)
		{
			newEnemy = make_shared<Elf>();
		}
		else if (enemyType <= 14)
		{
			newEnemy = make_shared<Orc>();
		}
		else if (enemyType <= 18)
		{
			newEnemy = make_shared<Merchant>(hostileMerchants);
		}
		else
		{
			cerr << "Error generating enemy" << endl;
		}

	}
	else
	{
		// Regular spawning probabilities

		// Cumulative probability
		// 2/9 human
		if (enemyType <= 4)
		{
			newEnemy = make_shared<Human>();
		}
		//
		else if (enemyType <= 7)
		{
			newEnemy = make_shared<Dwarf>();
		}
		else if (enemyType <= 12)
		{
			newEnemy = make_shared<Halfling>();
		}
		else if (enemyType <= 14)
		{
			newEnemy = make_shared<Elf>();
		}
		else if (enemyType <= 16)
		{
			newEnemy = make_shared<Orc>();
		}
		else if (enemyType <= 18)
		{
			newEnemy = make_shared<Merchant>(hostileMerchants);
		}
		else
		{
			cerr << "Error generating enemy" << endl;
		}
	}

	while (true)
	{
		// Generate a random chamber number (to ensure each chamber has equal probability)
		int targetChamberNum = theFloor->getRandomChamberNum();

		// Get random coordinates and its associated chamber
		int randX = theFloor->getRandomX();
		int randY = theFloor->getRandomY();
		int chamberNum = theFloor->chamberAt(randX, randY);

		if (chamberNum == targetChamberNum && !(game.isOccupied(randX, randY)))
		{
			newEnemy->setX(randX);
			newEnemy->setY(randY);

			return newEnemy;
		}
	}
	return nullptr;
}

void DefaultLevel::placePlayer(shared_ptr<Player> thePlayer)
{
	while (true)
	{
		int targetChamberNum = theFloor->getRandomChamberNum(); // select a random chamber number
		// Get random coordinates
		int randX = theFloor->getRandomX();
		int randY = theFloor->getRandomY();
		int chamberNum = theFloor->chamberAt(randX, randY);

		// Check the chamber
		if (!game.isOccupied(randX, randY) && chamberNum == targetChamberNum)
		{
			thePlayer->setX(randX);
			thePlayer->setY(randY);
			break;
		}
	}
}

shared_ptr<Potion> DefaultLevel::spawnPotionAt(int potionType, int x, int y)
{

	auto newPotion = make_shared<Potion>();

	switch (potionType)
	{
	case Potion::PotionTypes::RESTOREHEALTH:
	{
		newPotion = make_shared<RestoreHealth>();
		break;
	}
	case Potion::PotionTypes::POISONHEALTH:
	{
		newPotion = make_shared<PoisonHealth>();
		break;
	}
	case Potion::PotionTypes::BOOSTATK:
	{
		newPotion = make_shared<BoostAtk>();
		break;
	}
	case Potion::PotionTypes::WOUNDATK:
	{
		newPotion = make_shared<WoundAtk>();
		break;
	}
	case Potion::PotionTypes::BOOSTDEF:
	{
		newPotion = make_shared<BoostDef>();
		break;
	}
	case Potion::PotionTypes::WOUNDDEF:
	{
		newPotion = make_shared<WoundDef>();
		break;
	}
	}

	newPotion->setX(x);
	newPotion->setY(y);
	return newPotion;
}
shared_ptr<Gold> DefaultLevel::spawnGoldAt(int goldType, int x, int y, std::shared_ptr<Dragon> dragon)
{

	auto newGold = make_shared<Gold>("", 0);
	switch (goldType)
	{
	case Gold::GoldTypes::SMALL:
	{
		newGold = make_shared<SmallPile>();
		break;
	}
	case Gold::GoldTypes::MEDIUM:
	{
		newGold = make_shared<NormalPile>();
		break;
	}
	case Gold::GoldTypes::MERCHANT_HOARD:
	{
		newGold = make_shared<MerchantHoard>();
		break;
	}
	case Gold::GoldTypes::DRAGON_HOARD:
	{
		int numAttempts = 0;
		while (true)
		{

			auto dragonGold = make_shared<DragonHoard>();
			auto theDragon = make_shared<Dragon>(dragonGold);

			dragonGold->setX(x);
			dragonGold->setY(y);

			// Generate a dx and dy either -1 or 1
			int dY = rand() % 2 ? -1 : 1;
			int dX = rand() % 2 ? -1 : 1;

			int dragonX;
			int dragonY;
			// Too many failed generation attempts, just spawn somewhere
			if (numAttempts > 10)
			{
				dragonX = theFloor->getRandomX();
				dragonY = theFloor->getRandomY();
			}
			else
			{
				dragonX = dX + x;
				dragonY = dY + y;
			}

			// Set the coordinates for gold and dragon
			if (!(game.isOccupied(dragonX, dragonY)) && theFloor->chamberAt(dragonX, dragonY) == theFloor->chamberAt(x, y) && dragon != nullptr)
			{
				dragon->setHoard(dragonGold);
				dragon->setX(dragonX);
				dragon->setY(dragonY);
				return dragonGold;
			}

			// Requirement: to call spawnGoldAt with dragon hoard requires with an associated dragon
			if (dragon == nullptr)
			{
				return nullptr;
			}
		}
	}
	}

	newGold->setX(x);
	newGold->setY(y);

	return newGold;
}
