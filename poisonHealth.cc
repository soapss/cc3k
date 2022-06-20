#include "poisonHealth.h"
#include "potion.h"

PoisonHealth::PoisonHealth() : Potion{
    "Potion of Poison Health", 
    -10, 
    0, 
    0, 
    true,
    "PC was poisoned and lost HP."}
{}
