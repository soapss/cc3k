#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "floor.h"
#include "cell.h"
#include "color.h"


using namespace std; 

// Read the map from a filename and create the environment (2D vector of Cells)
void Floor::readMap(string filename) {

    // Reset the environment (this is necessary if we reset the level)
    
    environment.clear();

    string fileStr;
    std::ifstream infile(filename);

    int rowNum = 0;
    for (string line; getline(infile, line); ) {

        vector<Cell> row;
        for (int i = 0; i < line.length(); i++) {
            int chamberNum = chamberAt(rowNum, i);
            row.emplace_back(line[i], chamberNum);
        }
        environment.push_back(row);
        rowNum += 1;
    }

}


// Loop through the environment and return the char version of it
vector<vector<char>> Floor::getEnvironmentChar() {
    vector<vector<char>> charEnvironment;
    for (int i = 0; i < environment.size(); i++) {
        vector<char> charRow;
        for (int j = 0; j < environment[0].size(); j++) {
            charRow.push_back(environment[i][j].getChar());
        }
        charEnvironment.push_back(charRow);
    }
    return charEnvironment;
}

Cell Floor::cellAt(int x, int y) {
    return environment[y][x];
}

// Return the chamber number at a given tile
// Non-tiles (Walls, passages, doors, and empty cells) have no chamber number (-1)
// Chambers are numbered from top left to bottom right
int Floor::chamberAt(int x, int y) {
    // Chamber 1
    if ((x >= 3 && x <= 28) && (y >= 3 && y <= 6))
    {
        return 1;
    }
    // Chamber 2
	if (((x >= 39 && x <= 61) && (y >= 3 && y <= 6)) ||
	   ((x >= 61 && x <= 75) && (y >= 7 && y <= 12)) ||
	   ((x >= 62 && x <= 69) && (y == 5)) 			 ||
	   ((x >= 62 && x <= 72) && (y == 6)))
    {
		return 2;
    }
    // Chamber 3
    else if ((x >= 38 && x <= 49) && (y >= 10 && y <=12))
    {
        return 3;
    }
    // Chamber 4
    else if ((x >= 4 && x <= 24) && (y >= 15 && y <=21)) 
    {
        return 4;
    }
    // Chamber 5
    else if (
            (x >= 65 && x <= 75) && (y >= 16 && y <= 18)||
            (x >= 37 && x <= 75) && (y >= 19 && y <= 21) )
    {
        return 5;
    }
    // Else not a chamber
    return -1;
}

int Floor::chamberAt(shared_ptr<Entity> e) {
	return chamberAt(e->getX(), e->getY());
}

int Floor::getRandomChamberNum()
{
    // Generate a number between range [1, NUM_CHAMBERS]
    return rand() % NUM_CHAMBERS + 1;
}

int Floor::getRandomX()
{
    return rand() % WIDTH;
}
int Floor::getRandomY()
{
    return rand() % HEIGHT;
}

int Floor::getHeight() {
    return HEIGHT;
}

int Floor::getWidth() {
    return WIDTH;
}
