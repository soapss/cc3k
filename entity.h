#ifndef ENTITY_H
#define ENTITY_H
#include "color.h"
class Entity {
    int x;
    int y;
    char symbol;
    std::string color;
  public:
    Entity(int x, int y, char sym);
    Entity(char sym = ' ');
    Entity(char sym, std::string color);

    int getX();
    int getY();
    void setX(int newX);
    void setY(int newY);
    std::string getColor();
    char getSymbol();
    bool occupies(int x, int y);
};
#endif
