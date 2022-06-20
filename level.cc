#include "level.h"
using namespace std;

Level::Level(shared_ptr<Floor> floorMap): theFloor{floorMap} {}

Level::~Level() {};
