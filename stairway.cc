#include "entity.h"
#include "stairway.h"

Stairway::Stairway() : Entity{0, 0, '\\'} {}
Stairway::Stairway(int x, int y) : Entity{x, y, '/'} {}

