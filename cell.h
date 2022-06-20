#ifndef CELL_H
#define CELL_H
#include "entity.h"
class Cell {
    char ch;
    int chamberNum;



    public:
        Cell();
        Cell(char c, int chamberNum);
        char getChar();
        int getChamberNum();

    // Constants for cell types
    static const char WALL_HORIZONTAL = '|';
    static const char WALL_VERTICAL = '-';
    static const char TILE = '.';
    static const char PASSAGE = '#';
    static const char DOOR = '+';
    static const char NOTHING = ' ';


};
#endif
