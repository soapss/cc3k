#include "item.h"
#include "entity.h"
#include <string>
using namespace std; 
Item::Item(string name, char symbol, std::string color) : Entity{symbol, color}, name{name} {}

string Item::getName() {
    return name;
}
