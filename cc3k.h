#ifndef CC3K_H
#define CC3K_H
#include "floor.h"
#include "player.h"
#include "enemy.h"
#include "potion.h"
#include "stairway.h"
#include "gold.h"
#include <vector>
#include <string>
#include <memory>
#include "observer.h"
#include "subject.h"
#include "message.h"

class Level;

class CC3K : public Subject {
    int levelNum;
    std::shared_ptr<Floor> theFloor;
    std::shared_ptr<Level> theLevel;
    std::vector<Observer*> observers;

    std::shared_ptr<Stairway> theStairway;

    std::shared_ptr<Player> thePlayer;

    // A vector of 2D chars to store the raw levels
    std::vector<std::vector<std::vector<char>>> rawCustomLevels;
    bool isCustom;

    // Vector of all potions on the floor
    std::vector<std::shared_ptr<Potion>> thePotions;

    // Vector of all gold piles on floor
    std::vector<std::shared_ptr<Gold>> theGold;

    // Vector of all the enemies
    std::vector<std::shared_ptr<Enemy>> theEnemies;

    // The message displayed after the player makes an action
    std::vector<Message> messages;

    // Starting race (value is specified via Player::RaceTypes enum)
    int startingRace;

    // Amount of gold that the player has collected
    int playerGold;

    // Stops enemies from moving until this key is set again;
    bool stopEnemies;

    // Check if the current game has hostile merchants
    bool isHostileMerchants;


    // Number of potions spawned on each floor
    static const int NUM_POTIONS = 10;

    // Number of gold piles spawned on each floor
    static const int NUM_GOLD = 10;

    // Number of enemies spawned on each floor
    static const int NUM_ENEMIES = 20;

    // A 2D char vector for displaying the board
    std::vector<std::vector<char>> theDisplay;

    void moveAndAttackEnemies();
    void moveAndAttackEnemy(std::shared_ptr<Enemy> enemy);
    void checkPlayerDead();
    void spawnGoldPile(int goldType, int sourceX, int sourceY);

    // Bonus: enable fog view
    // Default false
    bool isFog;
    // Bonus: enable gameplay-related dlc
    // E.g. merchant, extra items, etc.
    bool isDLC;

    int computeFinalScore();




    public:
        void newGame();
        void newLevel();
        void parseCustomLevels(std::string filename);
        void loadCustomLevel(int customLevelNum);
        void render();
        void movePlayer(std::string dir);
        void usePotion(std::string dir);
        void playerAttack(std::string dir);
        void toggleStopEnemies();
        void spawnGoldPileAt(int goldType, int sourceX, int sourceY);
        bool isOccupied(int x, int y) const;
        bool isOccupiedOrNotChamber(int x, int y) const;
        bool isGameOver;
        bool isGameComplete;

        CC3K();
        ~CC3K();
        void setStartingRace(int newRace);
        std::pair<char,std::string> getState(int x, int y) override;
        std::string getGameStatus();
        std::vector<Message> getMessages();
        void addMessage(std::string text, std::string color);

        // BONUS FEATURES

        // Used for bonus credit only (fog)
        bool getFog();
        void setFog(bool newFog);
        std::shared_ptr<Player> getPlayer();
        // Buy from a merchant
        void useMerchant(std::string dir);
        
        bool getDLC() const;
        void setDLC(bool newDLC);

        



        
};
#endif
