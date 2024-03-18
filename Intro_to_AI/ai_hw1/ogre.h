#pragma once
#include "entity.h"
#include "actman.h"
#include <memory>

class Ogre : public Entity
{
  public:
    Ogre(Position position) 
    : Entity(position.getRow(), position.getColumn(), Sprites::ogre){};

    void move(Map& map, Actman& actman);
    void onDeath(Map& map, Actman& actman);
    void canMove(Map& map, Actman& actman);
};
