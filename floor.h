#ifndef FLOOR_H
#define FLOOR_H

#include <vector>
#include <string>
#include <memory>
#include "cell.h"

class Entity;

class Floor {

    // Environment is the tiles, walls, passage, etc.
    std::vector<std::vector<Cell>> environment;

    // Hardcoded board sizes
    static const int WIDTH = 79;
    static const int HEIGHT = 25;
    static const int NUM_CHAMBERS = 5;

    public:
        void readMap(std::string filename);

        int chamberAt(int x, int y);
        int chamberAt(std::shared_ptr<Entity> e);
        int getRandomChamberNum();
        int getRandomX();
        int getRandomY();

        Cell cellAt(int x, int y);

        int getWidth();
        int getHeight();

        // Return the environment vector as chars (not Cells)
        // for game display
        std::vector<std::vector<char>> getEnvironmentChar();
};

#endif
