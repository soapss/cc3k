#include "cc3k.h"
#include "level.h"
#include "defaultlevel.h"
#include <math.h>

#include "player.h"
#include "drow.h"
#include "shade.h"
#include "goblin.h"
#include "troll.h"
#include "vampire.h"

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

#include "potion.h"
#include "stairway.h"
#include "floor.h"

#include "cell.h"
#include "color.h"

#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <memory>
#include <map>
#include <sstream>
#include <fstream>

using namespace std;

CC3K::CC3K() : levelNum{1}, theFloor{make_shared<Floor>()},
               theLevel{make_shared<DefaultLevel>(theFloor, *this)},
               theStairway{nullptr}, playerGold{0},
               startingRace{Player::RaceTypes::SHADE}, stopEnemies{false}, isHostileMerchants{false}, isCustom{false},
               isFog{false}, isDLC{false}, isGameOver{false}, isGameComplete{false}
{
}

CC3K::~CC3K()
{
    for (int i = 0; i < observers.size(); i++)
    {
        detach(observers[i]);
    }
    observers.clear();
}

void CC3K::parseCustomLevels(std::string filename)
{
    isCustom = true;

    // Reads a file containing five levels and potions and gold placed already

    string fileStr;
    std::ifstream infile(filename);

    // Read five levels

    int rowNum = 0;

    // Read lines from a level
    vector<vector<char>> singleRawLevel;
    for (string line; getline(infile, line);)
    {
        // Read a char from the line
        vector<char> levelLine;
        for (int j = 0; j < line.size(); j++)
        {
            levelLine.push_back(line[j]);
        }
        singleRawLevel.push_back(levelLine);

        if (theFloor->getHeight() - 1 == rowNum)
        {
            // Make a temporary copy of the vector
            vector<vector<char>> tempLine;
            for (int z = 0; z < singleRawLevel.size(); z++)
            {
                tempLine.push_back(singleRawLevel[z]);
            }
            rawCustomLevels.push_back(tempLine);
            singleRawLevel.clear();
            rowNum = 0;
        }
        else
        {
            rowNum += 1;
        }
    }
}

void CC3K::loadCustomLevel(int customLevelNum)
{
    // Creates a new level
    // With the raw level data at customLevelNum

    vector<vector<char>> customLevelRaw = rawCustomLevels[customLevelNum];

    thePotions.clear();
    theGold.clear();
    theEnemies.clear();

    // Get the map and read it into a floor
    theFloor->readMap("map.txt");

    // If we're not on the first level, the player may have some permanent potions
    // So we should apply them and discard the effects of the temporary potions
    thePlayer->applyPermanentPotions();

    // Make two passes
    // First pass to generate dragon hoard + dragon
    // Second pass to generate everything else
    for (int passType = 0; passType < 2; passType++)
    {
        // Loop through and generate potions and gold
        for (int i = 0; i < customLevelRaw.size(); i++)
        {
            for (int j = 0; j < customLevelRaw[0].size(); j++)
            {
                // Spawn dragonhoard first
                if (passType == 1)
                {
                    switch (customLevelRaw[i][j])
                    {
                    // Spawn RH
                    case '0':
                    {
                        thePotions.push_back(theLevel->spawnPotionAt(Potion::PotionTypes::RESTOREHEALTH, j, i));
                        break;
                    }
                    // Spawn BA
                    case '1':
                    {
                        thePotions.push_back(theLevel->spawnPotionAt(Potion::PotionTypes::BOOSTATK, j, i));
                        break;
                    }
                    // Spawn BD
                    case '2':
                    {
                        thePotions.push_back(theLevel->spawnPotionAt(Potion::PotionTypes::BOOSTDEF, j, i));
                        break;
                    }
                    // Spawn PH
                    case '3':
                    {
                        thePotions.push_back(theLevel->spawnPotionAt(Potion::PotionTypes::POISONHEALTH, j, i));
                        break;
                    }
                    // Spawn WA
                    case '4':
                    {
                        thePotions.push_back(theLevel->spawnPotionAt(Potion::PotionTypes::WOUNDATK, j, i));
                        break;
                    }
                    // Spawn WD
                    case '5':
                    {
                        thePotions.push_back(theLevel->spawnPotionAt(Potion::PotionTypes::WOUNDDEF, j, i));
                        break;
                    }
                    // Spawn Normal gold pile
                    case '6':
                    {
                        theGold.push_back(theLevel->spawnGoldAt(Gold::GoldTypes::MEDIUM, j, i, nullptr));
                        break;
                    }
                    // Spawn small hoard
                    case '7':
                    {
                        theGold.push_back(theLevel->spawnGoldAt(Gold::GoldTypes::SMALL, j, i, nullptr));
                        break;
                    }
                    // Spawn merchant hoard
                    case '8':
                    {
                        theGold.push_back(theLevel->spawnGoldAt(Gold::GoldTypes::MERCHANT_HOARD, j, i, nullptr));
                        break;
                    }
                    case '@':
                    {
                        thePlayer->setX(j);
                        thePlayer->setY(i);
                        break;
                    }
                    case '\\':
                    {
                        theStairway = make_shared<Stairway>();
                        theStairway->setX(j);
                        theStairway->setY(i);
                    }
                    // Spawn enemies
                    case 'W':
                    {
                        auto dwarf = make_shared<Dwarf>();
                        dwarf->setX(j);
                        dwarf->setY(i);
                        theEnemies.push_back(dwarf);
                        break;
                    }
                    case 'H':
                    {
                        auto human = make_shared<Human>();
                        human->setX(j);
                        human->setY(i);
                        theEnemies.push_back(human);
                        break;
                    }
                    case 'O':
                    {
                        auto orc = make_shared<Orc>();
                        orc->setX(j);
                        orc->setY(i);
                        theEnemies.push_back(orc);
                        break;
                    }
                    case 'L':
                    {
                        auto halfling = make_shared<Halfling>();
                        halfling->setX(j);
                        halfling->setY(i);
                        theEnemies.push_back(halfling);
                        break;
                    }
                    case 'M':
                    {
                        auto merchant = make_shared<Merchant>(false);
                        merchant->setX(j);
                        merchant->setY(i);
                        theEnemies.push_back(merchant);
                        break;
                    }
                    case 'E':
                    {
                        auto elf = make_shared<Elf>();
                        elf->setX(j);
                        elf->setY(i);
                        theEnemies.push_back(elf);
                        break;
                    }
                    default:
                        break;
                    }
                }
                else
                {
                    switch (customLevelRaw[i][j])
                    {
                    // Spawn dragon hoard
                    case '9':
                    {

                        auto dragon = make_shared<Dragon>(nullptr);
                        // Make the dragon hoard
                        auto dragonHoard = make_shared<DragonHoard>();
                        dragon->setHoard(dragonHoard);

                        theGold.push_back(theLevel->spawnGoldAt(Gold::GoldTypes::DRAGON_HOARD, j, i, dragon));

                        int dragonX;
                        int dragonY;
                        // search all 8 cells for the dragon
                        // east
                        if (customLevelRaw[i][j-1] == 'D')
                        {
                            dragonX = j-1;
                            dragonY = i;
                        }
                        // west
                        else if (customLevelRaw[i][j+1] == 'D')
                        {
                            dragonX = j+1;
                            dragonY = i;
                        }
                        // south
                        else if (customLevelRaw[i+1][j] == 'D')
                        {
                            dragonX = j;
                            dragonY = i+1;
                        }
                        // north
                        else if (customLevelRaw[i-1][j] == 'D')
                        {
                            dragonX = j;
                            dragonY = i-1;
                        }
                        // northeast
                        else if (customLevelRaw[i-1][j-1] == 'D')
                        {
                            dragonX = j-1;
                            dragonY = i-1;
                        }
                        // northwest
                        else if (customLevelRaw[i-1][j+1] == 'D')
                        {
                            dragonX = j+1;
                            dragonY = i-1;
                        }
                        // southeast
                        else if (customLevelRaw[i+1][j-1] == 'D')
                        {
                            dragonX = j-1;
                            dragonY = i+1;
                        }
                        // southwest
                        else if (customLevelRaw[i][j+1] == 'D')
                        {
                            dragonX = j+1;
                            dragonY = i;
                        }
                        else
                        {
                            cerr << "Invalid input: No dragon found adjacent to the dragon hoard!" << endl;
                        }

                        dragon->setX(dragonX);
                        dragon->setY(dragonY);
                        if (dragon)
                        {
                            theEnemies.push_back(dragon);
                        }
                        break;
                    }
                    default:
                    {
                        break;
                    }
                    }
                }
            }
        }
    }


}

void CC3K::newGame()
{
    levelNum = 1;
    playerGold = 0;
    isGameOver = false;
    isGameComplete = false;
    // Generate the player
    messages.emplace_back("Player character has spawned. ", Color::GREEN);
    thePlayer = theLevel->generatePlayer(startingRace);

    // Generate the level
    newLevel();
}

void CC3K::newLevel()
{
    // If the game uses custom levels, load them instead
    if (isCustom)
    {
        loadCustomLevel(levelNum-1);
        return;
    }
    /*
        20 enemies are spawned per floor (this number does not include dragons). Every
        chamber is equally likely to spawn any particular monster (similarly for floor
        tiles). We require that generation happens in the following order: player
        character location, stairway location, potions, gold, enemies. This is to allow
        us to more easily evaluate that your random generation is correctly implemented.
        Note that multiple objects (enemies, gold, and potions) cannot occupy the same
        cell on the game board. That is, no two objects can ever occupy the same space.
        The one exception to this is the case of gold. Typically, when a player
        character walks over gold, it is picked up. The exception to this is if the gold
        is associated with a still alive dragon; in this case, the player simply walks
        over the gold, without picking it. When the PC attempts to move on to a
        stairway, the next level is instead generated and displayed, with the PC
        spawning in a random position on the new level. Items and enemies should only
        ever spawn on a floor tile and never in a doorway, passage, or the stairs
        leading down to the next floor.
    */
    thePotions.clear();
    theGold.clear();
    theEnemies.clear();

    // Get the map and read it into a floor
    theFloor->readMap("map.txt");

    // If we're not on the first level, the player may have some permanent potions
    // So we should apply them and discard the effects of the temporary potions
    thePlayer->applyPermanentPotions();

    // Place the player at a random chamber
    theLevel->placePlayer(thePlayer);

    // Generate stairway
    int chamberPlayer = theFloor->chamberAt(thePlayer);
    theStairway = theLevel->generateStairway(chamberPlayer);

    // Generate potions
    for (int i = 0; i < NUM_POTIONS; i++)
    {
        thePotions.push_back(theLevel->generatePotion());
    }

    // Generate gold
    for (int i = 0; i < NUM_GOLD; i++)
    {
        auto newDragon = make_shared<Dragon>(nullptr);
        shared_ptr<Gold> newGold = theLevel->generateGold(newDragon);
        theGold.push_back(newGold);
        if (newDragon->getHoard())
        {
            theEnemies.push_back(newDragon);
        }
    }

    // Generate enemies
    for (int i = 0; i < NUM_ENEMIES; i++)
    {
        theEnemies.push_back(theLevel->generateEnemy(isHostileMerchants));
    }
}

// Helper function to compute new position after moving one unit in the specified direction
pair<pair<int, int>, string> getPosAtDirection(int x, int y, string dir)
{
    int dX = 0;
    int dY = 0;
    string direction;
    if (dir == "no")
    {
        dY = -1;
        direction = "North";
    }

    else if (dir == "so")
    {
        dY = 1;
        direction = "South";
    }

    else if (dir == "ea")
    {
        dX = 1;
        direction = "East";
    }

    else if (dir == "we")
    {
        dX = -1;
        direction = "West";
    }

    else if (dir == "ne")
    {
        dY = -1;
        dX = 1;
        direction = "Northeast";
    }
    else if (dir == "nw")
    {
        dY = -1;
        dX = -1;
        direction = "Northwest";
    }

    else if (dir == "se")
    {
        dY = 1;
        dX = 1;
        direction = "Southeast";
    }

    else if (dir == "sw")
    {
        dY = 1;
        dX = -1;
        direction = "Southwest";
    }
    else
    {
        cerr << "Invalid direction" << endl;
    }
    return make_pair(make_pair(x + dX, y + dY), direction);
}

// Returns true if an entity is occupying a cell
bool CC3K::isOccupied(int x, int y) const
{
    auto occupies = [&x, &y](shared_ptr<Entity> e)
    { return e->occupies(x, y); };
    return ((thePlayer && thePlayer->occupies(x, y)) ||
            (theStairway && theStairway->occupies(x, y)) ||
            any_of(thePotions.begin(), thePotions.end(), occupies) ||
            any_of(theGold.begin(), theGold.end(), occupies) ||
            any_of(theEnemies.begin(), theEnemies.end(), occupies));
}

bool CC3K::isOccupiedOrNotChamber(int x, int y) const
{
    return isOccupied(x, y) || theFloor->chamberAt(x,y) == -1;
}

void CC3K::checkPlayerDead()
{
    if (thePlayer->getHP() <= 0)
    {
        isGameOver = true;
        messages.emplace_back("You died! Press 'r' to play again or any other key to quit.", Color::RED);
    }
}

void CC3K::movePlayer(string dir)
{
    // Apply any passive ability from the player
    thePlayer->abilityPassive();

    pair<pair<int, int>, string> newPos = getPosAtDirection(thePlayer->getX(), thePlayer->getY(), dir);
    // Compute the new position

    int newX = newPos.first.first;
    int newY = newPos.first.second;

    // Check if the new position is a stairway FIRST
    // In this case, go to next level
    if (newX == theStairway->getX() && newY == theStairway->getY())
    {
        // Increase level number
        levelNum += 1;


        // Check if we are past 5 levels 
        if (levelNum > 5)
        {
            isGameComplete = true;
            messages.emplace_back("Congratulations, you've beaten the game! Press 'r' to play again or any other key to quit. \n", Color::GREEN);
            messages.emplace_back("Your final score is: " + to_string(computeFinalScore()), Color::YELLOW);
            return;
        }

        // Respawn everything
        newLevel();

        messages.emplace_back("Now entering level " + to_string(levelNum) + "!", Color::BOLDYELLOW);

        return;
    }

    // Next, check if player moved onto gold
    // Check if there exists a potion in the specified direction
    int foundGold = -1;
    for (int i = 0; i < theGold.size(); i++)
    {
        if (newX == theGold[i]->getX() && newY == theGold[i]->getY())
        {
            foundGold = i;
            break;
        }
    }
    // If the player is walking on gold
    if (foundGold != -1)
    {
        messages.emplace_back("PC moves " + newPos.second + ".", Color::BOLDMAGENTA);

        // Check dragon hoard
        // The condition will be true whenever gold is not a dragon hoard
        // Or when it is a dragon hoard AND the associated dragon is slain

        if (thePlayer->getName() == "Dragonslayer" || (!(theGold[foundGold]->getName() == "Dragon Hoard" && !theGold[foundGold]->getPickup())))
        {
            // If normal gold, we collect it and remove it

            // Add the gold to player
            playerGold += theGold[foundGold]->getValue();

            // Add the message
            messages.emplace_back("PC picks up a " +
                                      theGold[foundGold]->getName() + " worth " +
                                      to_string(theGold[foundGold]->getValue()) + " gold.",
                                  Color::GREEN);

            // Remove the gold from the map
            theGold.erase(theGold.begin() + foundGold);
        }
        // Move the player over the gold
        thePlayer->setX(newX);
        thePlayer->setY(newY);

        // The enemies now move and attack the player if in range
        moveAndAttackEnemies();

        return;
    }

    // Else, not a stairway or gold, we can use isOccupied() properly
    // Check if the new position is a tile
    // (i.e. don't move into a wall or empty space)
    if (isOccupied(newX, newY) ||
        (theFloor->cellAt(newX, newY).getChar() != Cell::TILE &&
         theFloor->cellAt(newX, newY).getChar() != Cell::DOOR &&
         theFloor->cellAt(newX, newY).getChar() != Cell::PASSAGE))
    {
        messages.emplace_back("You cannot move there.", Color::RED);
        // The enemies now move and attack the player if in range
        moveAndAttackEnemies();

        return;
    }

    // Else the new position is valid, move player there
    thePlayer->setX(newX);
    thePlayer->setY(newY);
    messages.emplace_back("PC moves " + newPos.second + ".", Color::BOLDMAGENTA);

    // The enemies now move and attack the player if in range
    moveAndAttackEnemies();
}

// Use a potion in the direction specified (from the player)
void CC3K::usePotion(string dir)
{
    // Apply any passive ability from the player
    thePlayer->abilityPassive();

    // Get the coordinates to check
    pair<pair<int, int>, string> checkPotion = getPosAtDirection(thePlayer->getX(), thePlayer->getY(), dir);
    pair<int, int> checkPotionPos = checkPotion.first;

    // Check if there exists a potion in the specified direction
    int found = -1;
    for (int i = 0; i < thePotions.size(); i++)
    {
        if (thePotions[i]->getX() == checkPotionPos.first && thePotions[i]->getY() == checkPotionPos.second)
        {
            found = i;
            break;
        }
    }

    // If we did not find a potion at that location
    if (found == -1)
    {
        messages.emplace_back("There is no potion in that direction.", Color::RED);
        return;
    }
    // Potion is found, apply the potion and erase it from the potion vector
    else
    {
        // Apply the effect to the player
        thePlayer->applyPotion(thePotions[found]);

        // Output message
        // E.g. "You used a Potion of Restore Health"
        messages.emplace_back("PC used a " + thePotions[found]->getName() + ". " + thePotions[found]->getDescription(),
                              Color::BOLDMAGENTA);

        // Erase the potion from the vector
        thePotions.erase(thePotions.begin() + found);
    }
}


// Buy from a merchant in a direction specified (from the player)
void CC3K::useMerchant(string dir)
{
    // Apply any passive ability from the player
    thePlayer->abilityPassive();

    // Get the coordinates to check
    pair<pair<int, int>, string> checkMerchant = getPosAtDirection(thePlayer->getX(), thePlayer->getY(), dir);
    pair<int, int> checkMerchantPos = checkMerchant.first;

    // Check if there exists a potion in the specified direction
    int found = -1;
    for (int i = 0; i < theEnemies.size(); i++)
    {
        if (theEnemies[i]->getX() == checkMerchantPos.first && theEnemies[i]->getY() == checkMerchantPos.second && theEnemies[i]->getName() == "Merchant")
        {
            found = i;
            break;
        }
    }

    // If we did not find a Merchant at that location
    if (found == -1)
    {
        messages.emplace_back("There is no Merchant in that direction.", Color::RED);
        return;
    }
    // Merchant is found, buy 
    else
    {
        // If we have insufficient funds
        if (playerGold < 3)
        {
            messages.emplace_back("You need at least 3 Gold to purchase from the Merchant.",
                            Color::RED);
            return;
        }
        if (isHostileMerchants)
        {
            messages.emplace_back("You've already angered all merchants and they refuse to sell to you!",
                            Color::RED);
            return;
        }
        else 
        {
            shared_ptr<Potion> boughtPotion;
            int gen = rand() % 3;

            if (gen == 0)
            {
                boughtPotion = make_shared<RestoreHealth>();
            }
            else if (gen == 1)
            {
                boughtPotion = make_shared<BoostAtk>();
            }
            else if (gen == 2)
            {
                boughtPotion = make_shared<BoostDef>();
            }
            // Apply the effect to the player
            thePlayer->applyPotion(boughtPotion);

            playerGold -= 3;

            // Output message
            messages.emplace_back("You bought a potion from the Merchant for 3 Gold.",
                            Color::BOLDMAGENTA);
            messages.emplace_back("PC used a " + boughtPotion->getName() + ". " + boughtPotion->getDescription(),
                                Color::BOLDMAGENTA);
        }

    }
}

void CC3K::render()
{
    notifyObservers();
    // Clear the messages
    messages.clear();
}

int CC3K::computeFinalScore()
{
    if (startingRace == Player::RaceTypes::SHADE)
    {
        return ceil((float)playerGold * 1.5);
    }
    else
    {
        return playerGold;
    }
}

string CC3K::getGameStatus()
{
    string ret = "";
    ret += "Race: " + thePlayer->getName() + " ";

    // Print out player gold
    ret += "Gold: " + to_string(playerGold);

    ret += "                                                   ";

    ret += "Floor: " + to_string(levelNum) + "\n";

    // Print out HP
    ret += "HP: " + to_string(thePlayer->getHP()) + "\n";

    // Print out Atk
    ret += "Atk: " + to_string(thePlayer->getAtk()) + "\n";

    // Print out Def
    ret += "Def: " + to_string(thePlayer->getDef()) + "\n";

    // DLC Feature: Show exp
    if (getDLC())
    {
        ret += "XP: " + to_string(thePlayer->getExp()) + "   Lvl: " + to_string(thePlayer->getLvl()) + "\n";
    }

    // Print out action (for any actions) that occur
    ret += "Action:";



    return ret;
}

pair<char, string> CC3K::getState(int x, int y)
{
    // Check the player coordinates
    if (thePlayer->getX() == x && thePlayer->getY() == y)
    {
        return make_pair(thePlayer->getSymbol(), thePlayer->getColor());
    }

    // Check the staircase coordinates
    if (theStairway->getX() == x && theStairway->getY() == y)
    {
        return make_pair(theStairway->getSymbol(), theStairway->getColor());
    }

    // Check all potions
    for (auto potion : thePotions)
    {
        if (potion->getX() == x && potion->getY() == y)
        {
            return make_pair(potion->getSymbol(), potion->getColor());
        }
    }

    // Check all gold
    for (auto gold : theGold)
    {
        if (gold->getX() == x && gold->getY() == y)
        {
            return make_pair(gold->getSymbol(), gold->getColor());
        }
    }
    // Check enemies
    for (auto enemy : theEnemies)
    {
        if (enemy->getX() == x && enemy->getY() == y)
        {
            return make_pair(enemy->getSymbol(), enemy->getColor());
        }
    }
    // Else, return the floor element
    return make_pair(theFloor->cellAt(x, y).getChar(), Color::RESET);
}

void CC3K::toggleStopEnemies()
{
    stopEnemies = !stopEnemies;
    if (stopEnemies)
    {
        messages.emplace_back("Enemies are frozen now!", Color::BLUE);
    }
    else
    {
        messages.emplace_back("Enemies are unfrozen now!", Color::BLUE);
    }
}

// Move all the enemies and attack the player if they are in range
void CC3K::moveAndAttackEnemies()
{
    if (stopEnemies)
    {
        return;
    }

    // "starting at the leftmost enemy, move all enemies on that row and then move
    // to the next row starting with the leftmost. Any particular enemy should only be moved once per player action (e.g. moving
    // to a line that has not been processed does not grant an extra move)."

    // This makes the implementation more complicated than simply looping through the vector of enemies

    vector<shared_ptr<Enemy>> enemiesCopy = theEnemies;

    // Create a hash map to mark the xy coordinates of the new enemy locations
    std::map<std::pair<int, int>, bool> checkXY;

    for (int i = 0; i < theFloor->getHeight(); i++)
    {
        for (int j = 0; j < theFloor->getWidth(); j++)
        {
            // Skip non-chamber tiles since enemies cannot be there
            if (theFloor->chamberAt(j, i) == -1)
            {
                continue;
            }

            // Else search for an enemy
            else
            {
                for (int k = 0; k < theEnemies.size(); k++)
                {
                    if (checkXY.count(make_pair(j, i)) == 0 && i == theEnemies[k]->getY() && j == theEnemies[k]->getX())
                    {
                        moveAndAttackEnemy(theEnemies[k]);
                        checkXY[make_pair(theEnemies[k]->getX(), theEnemies[k]->getY())] = true;
                    }
                }
            }
        }
    }

    // Check if the player has died from enemy attacks
    checkPlayerDead();
}

void CC3K::moveAndAttackEnemy(shared_ptr<Enemy> enemy)
{

    // Pathfinder logic
    if (enemy->getName() == "Pathfinder" && 
    theFloor->chamberAt(enemy->getX(), enemy->getY()) == theFloor->chamberAt(thePlayer->getX(), thePlayer->getY()))
    {
        pair<int, int> newPos = Pathfinder::bfs(*this, enemy->getX(), enemy->getY());
        int pathfinderDx = newPos.first - enemy->getX();
        int pathfinderDy = newPos.second - enemy->getY();
        if (!isOccupied(newPos.first, newPos.second))
        {
            enemy->move(pathfinderDx, pathfinderDy );
        }

    }
    else 
    {

        // Move the enemy
        // Generate a random -1, 0, 1
        int deltaX = rand() % 3 - 1;
        int deltaY = rand() % 3 - 1;
        int newX = enemy->getX() + deltaX;
        int newY = enemy->getY() + deltaY;
        if (!isOccupied(newX, newY) && theFloor->cellAt(newX, newY).getChar() == Cell::TILE)
        {
            enemy->move(deltaX, deltaY);
        }
    }


    // Attack the player if they are in range

    // If merchant and not triggered hostile, skip (don't attack)
    if (enemy->getName() == "Merchant" && !isHostileMerchants)
    {
        return;
    }
    if (enemy->inRange(thePlayer))
    {
        // Number of times that the enemy can attack the player
        // Default is 1, 2 if enemy is elf
        int numAttacks = 1;
        if (enemy->getName() == "Elf" && thePlayer->getName() != "Drow")
        {
            numAttacks = 2;
        }
        for (int numAttack = 0; numAttack < numAttacks; numAttack++)
        {
            // Attack the pc if it is in range
            int dmg = enemy->attack(thePlayer);
            if (dmg == 0)
            {
                messages.emplace_back(enemy->getName() + " tried to attack PC but missed.", Color::RED);
            }
            else
            {
                messages.emplace_back(enemy->getName() + " deals " + to_string(dmg) + " damage to PC.", Color::RED);
            }
        }
    }
}

void CC3K::playerAttack(string cmd)
{
    // Apply any passive ability from the player
    thePlayer->abilityPassive();

    pair<pair<int, int>, string> pos = getPosAtDirection(thePlayer->getX(), thePlayer->getY(), cmd);
    // The coordinates where the player is striking the possible enemy
    int attackX = pos.first.first;
    int attackY = pos.first.second;

    // To keep track of if player hit an enemy
    int haveHit = false;

    for (int i = 0; i < theEnemies.size(); i++)
    {
        // Check if the player is in range of attacking the enemy
        if (theEnemies[i]->inRange(thePlayer) && theEnemies[i]->getX() == attackX && theEnemies[i]->getY() == attackY)
        {
            int dmg = thePlayer->attack(theEnemies[i]);
            haveHit = true;

            // Extra logic for vampire: If successful attack, it gains 5hp
            if (thePlayer->getName() == "Vampire" && dmg != 0)
            {
                if (theEnemies[i]->getName() == "Dwarf")
                {
                    messages.emplace_back("PC is allergic to Dwarf and lost 5 HP.", Color::BLUE);
                }
                else
                {
                    messages.emplace_back("PC gains 5 HP as a vampire from the successful attack.", Color::BLUE);
                }
            }

            // If player misses (this is possible if halfling attacks player)
            if (dmg == 0)
            {
                messages.emplace_back("PC attacked but missed.", Color::MAGENTA);
            }

            // If the enemy's HP drops below 0, it is dead
            else if (theEnemies[i]->getHP() <= 0)
            {
                // Extra logic for merchant
                if (theEnemies[i]->getName() == "Merchant" && !isHostileMerchants)
                {
                    // All merchants are hostile now
                    isHostileMerchants = true;
                    messages.emplace_back("PC has slain " + theEnemies[i]->getName() + " (" + to_string(dmg) + " damage).", Color::YELLOW);
                    messages.emplace_back("You have drawn the ire of all merchants. They will be hostile to you from now on!", Color::MAGENTA);
                }
                // Extra logic for dragon: allow pickup the gold
                else if (theEnemies[i]->getName() == "Dragon")
                {
                    // Get the gold
                    Dragon &theDragon = dynamic_cast<Dragon &>(*theEnemies[i]);
                    auto dragonGold = theDragon.getHoard();

                    // Set the gold to allow pickup
                    dragonGold->setPickup(true);
                    messages.emplace_back("PC has slain " + theEnemies[i]->getName() + " (" + to_string(dmg) + " damage).", Color::YELLOW);
                    messages.emplace_back("The dragon's hoard is now yours to take. ", Color::GREEN);
                }
                // Otherwise standard enemy was slain
                else
                {
                    messages.emplace_back("PC has slain " + theEnemies[i]->getName() + " (" + to_string(dmg) + " damage).", Color::YELLOW);
                }

                // Humans drop two normal piles of gold
                if (theEnemies[i]->getName() == "Human")
                {
                    spawnGoldPileAt(Gold::GoldTypes::MEDIUM, theEnemies[i]->getX(), theEnemies[i]->getY());
                    spawnGoldPileAt(Gold::GoldTypes::MEDIUM, theEnemies[i]->getX(), theEnemies[i]->getY());
                    messages.emplace_back("The slain Human dropped two normal piles of gold.", Color::GREEN);
                }
                if (theEnemies[i]->getName() == "Merchant")
                {
                    spawnGoldPileAt(Gold::GoldTypes::MERCHANT_HOARD, theEnemies[i]->getX(), theEnemies[i]->getY());
                    messages.emplace_back("The slain Merchant dropped one Merchant Hoard.", Color::GREEN);
                }

                // Upon their demise, any enemy that is not a dragon, human, or merchant will drop either a small pile or normal pile of gold.
                // This gold is immediately added to the player character’s total.
                if (theEnemies[i]->getName() != "Dragon" && theEnemies[i]->getName() != "Human" && theEnemies[i]->getName() != "Merchant")
                {
                    int goldCollect = SmallPile::value;
                    if (rand() % 2)
                    {
                        goldCollect = NormalPile::value;
                    }

                    playerGold += goldCollect;

                    messages.emplace_back("PC collected " + to_string(goldCollect) + " gold from the slain " + theEnemies[i]->getName() + ".", Color::YELLOW);
                }

                // Goblin steals 5 gold from every slain enemy.
                if (thePlayer->getName() == "Goblin")
                {
                    playerGold += 5;
                    messages.emplace_back("PC's Goblin skills collect an extra 5 gold from the slain " + theEnemies[i]->getName() + ".", Color::YELLOW);
                }

                // DLC Feature: Add to player's exp
                if (getDLC())
                {
                    std::string result = thePlayer->addExp(5);
                    // If we levelled up, the result string is non-empty
                    if (result != "")
                    {
                        messages.emplace_back("You leveled up and permanently increased your max " + result + "!", Color::YELLOW);
                    }
                    
                }

                theEnemies[i] = theEnemies.back();
                theEnemies.pop_back();
            }

            // Player attacks enemy but enemy did not die yet
            else
            {
                messages.emplace_back("PC deals " + to_string(dmg) + " damage to " +
                                          theEnemies[i]->getName() + " (" + to_string(theEnemies[i]->getHP()) +
                                          " HP)" + ".",
                                      Color::CYAN);
            }
        }
    }
    if (!haveHit)
    {
        messages.emplace_back("There is nothing to attack there.", Color::BOLDMAGENTA);
    }

    moveAndAttackEnemies();
}

void CC3K::setStartingRace(int newRace)
{
    startingRace = newRace;
}

vector<Message> CC3K::getMessages()
{
    return messages;
}

void CC3K::addMessage(string text, string color)
{
    messages.emplace_back(text, color);
}

// Drop a gold pile around a source location
void CC3K::spawnGoldPileAt(int goldType, int sourceX, int sourceY)
{
    // Handling a rare edge case:
    // It is possible that the source location is too crowded and there is nowhere for the gold to spawn
    // If, after failed 10 attempts to spawn, we will just spawn the gold somewhere
    // else in the chamber
    int spawnAttempts = 0;

    auto newGold = make_shared<Gold>("", 0);

    switch (goldType)
    {
    case (Gold::GoldTypes::MERCHANT_HOARD):
    {
        newGold = make_shared<MerchantHoard>();
        break;
    }
    case (Gold::GoldTypes::MEDIUM):
    {
        newGold = make_shared<NormalPile>();
        break;
    }
    default:
    {
        cerr << "Invalid gold type spawn." << endl;
        return;
    }
    }
    bool isGenerating = true;
    while (isGenerating)
    {

        int targetChamberNum = theFloor->chamberAt(sourceX, sourceY);

        int randX;
        int randY;
        int deltaX;
        int deltaY;

        // The source location is too crowded, spawn it somewhere in the chamber
        if (spawnAttempts > 100)
        {
            randX = theFloor->getRandomX();
            randY = theFloor->getRandomY();
        }
        else
        {
            // Generate a random -1, 0, 1
            deltaX = rand() % 3 - 1;
            deltaY = rand() % 3 - 1;

            // Get random coordinates from the source
            randX = sourceX + deltaX;
            randY = sourceY + deltaY;
        }

        // Check the associated chamber to the random coordinates
        int chamberNum = theFloor->chamberAt(randX, randY);

        if (chamberNum == targetChamberNum && ((randX == sourceX && randY == sourceY) || (!isOccupied(randX, randY))))
        {
            newGold->setX(randX);
            newGold->setY(randY);
            theGold.push_back(newGold);
            isGenerating = false; // exit the loop
            // If generation fails, we do nothing
        }

        spawnAttempts += 1;
    }
}


std::shared_ptr<Player> CC3K::getPlayer()
{
    return thePlayer;
}

bool CC3K::getFog()
{
    return isFog;
}

void CC3K::setFog(bool newFog)
{
    isFog = newFog;
}

bool CC3K::getDLC() const
{
    return isDLC;
}

void CC3K::setDLC(bool newDLC)
{
    isDLC = newDLC;
}
