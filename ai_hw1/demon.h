#pragma once
#include "entity.h"
#include "actman.h"
#include <algorithm>
#include <memory>

class Demon :public Entity
{
  public:
    Demon(Position position) 
    : Entity(position.getRow(), position.getColumn(), Sprites::demon){};

    void move(Map& map, Actman& actman);
    void onDeath(Map& map, Actman& actman);
    void canMove(Map& map, Actman& actman);
};
