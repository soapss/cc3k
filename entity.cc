#include "entity.h"
#include "color.h"
#include <string>

using namespace std; 

Entity::Entity(char sym) : symbol{sym}, color{Color::RESET} {}
Entity::Entity(int x, int y, char sym) : x{x}, y{y}, symbol{sym}, color{Color::RESET} {}
Entity::Entity(char sym, std::string color) : symbol{sym}, color{color} {}

int Entity::getX() {
    return x;
}

int Entity::getY() {
    return y;
}

void Entity::setX(int newX) {
    x = newX;
}

void Entity::setY(int newY) {
    y = newY;
}

char Entity::getSymbol() {
    return symbol;
}

string Entity::getColor() {
    return color;
}

bool Entity::occupies(int xPos, int yPos) {
	return ((x == xPos) && (y == yPos));
}
