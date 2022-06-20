#include "cell.h"

Cell::Cell() : chamberNum{-1}, ch{Cell::NOTHING} {}

Cell::Cell(char c, int chamberNum) : chamberNum{chamberNum}, ch{c} {}

char Cell::getChar() {
    return ch;
}
int Cell::getChamberNum() {
    return chamberNum;
}
