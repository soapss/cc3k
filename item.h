#ifndef ITEM_H
#define ITEM_H
#include "entity.h"
#include <string>

class Item : public Entity {
    private:
        std::string name;
    public:
    Item(std::string name, char symbol, std::string color);
    void activate();
    std::string getName();

};

#endif
