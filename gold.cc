#include "gold.h"
#include <string>

Gold::Gold(std::string name, int value) : Item{name, 'G', Color::YELLOW}, value{value}, pickup{true} {}
Gold::Gold(std::string name, int value, bool pickup) : Item{name, 'G', Color::YELLOW}, value{value}, pickup{pickup} {}

int Gold::getValue() {
    return value;
}
bool Gold::getPickup() {
    return pickup;
}
void Gold::setPickup(bool newPickup) {
    pickup = newPickup;
}
