#include "entity.h"

Entity::Entity(int row, int col, Sprites tile)
  : position(row, col)
{
  sprite = static_cast<char>(tile);
  isAlive = true;
  hasActed = false;
}

Entity::~Entity()
{
  position.row = 0;
  position.column = 0;
  sprite = static_cast<char>(Sprites::space);
  isAlive = false;
  hasActed = true;
}

void Entity::move()
{

}

void Entity::onDeath()
{

}

bool Entity::validMove(Map map, int row, int col)
{
  bool isValid = false;
  
  if(!(map.getCell(row, col) == static_cast<char>(Sprites::wall)))
  {
    isValid = true;
  }
  else 
  {
    isValid = false;
  }

  return isValid;
}

Position Entity::getPosition()
{
  return position;
}

char Entity::getSprite()
{
  return sprite;
}

bool Entity::getAlive()
{
  return isAlive;
}

bool Entity::getActed()
{
  return hasActed;
}

void Entity::setPosition(int row, int col)
{
  position.row = row;
  position.column = col;
}

void Entity::setPosition(Position position)
{
  position = position;
}

void Entity::setSprite(char sprite)
{
  sprite = sprite;
}

void Entity::setAlive(bool alive)
{
  isAlive = alive;
}

void Entity::setActed(bool acted)
{
  hasActed = acted;
}
