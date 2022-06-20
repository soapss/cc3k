#include "merchant.h"
#include "enemy.h"
#include <iostream>
#include <cmath>
using namespace std;

Merchant::Merchant(bool isHostile) : Enemy{30, 70, 5, isHostile, 'M', "Merchant"} {}

