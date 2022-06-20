#ifndef GOLD_H
#define GOLD_H
#include "item.h"
#include <string>
class Gold : public Item {
    int value;
    bool pickup;
    public:
    Gold(std::string name, int value);
    Gold(std::string name, int value, bool pickup);
    int getValue();
    bool getPickup();
    void setPickup(bool newPickup);

    const static int NUM_GOLD_TYPES = 4;

    enum GoldTypes {
        SMALL = 1,
        MEDIUM,
        MERCHANT_HOARD,
        DRAGON_HOARD,
    };

};
#endif
