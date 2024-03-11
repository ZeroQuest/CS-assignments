#pragma once
#include <vector>
#include "map.h"

class Entity
{
  public:
    Entity(int row, int col, Sprites tile);
    ~Entity();

    virtual void move();
    virtual void onDeath();

    bool validMove(Map map, int row, int col);

    Position getPosition();
    char getSprite();
    bool getAlive();
    bool getActed();

    void setPosition(int row, int col);
    void setPosition(Position position);
    void setSprite(char sprite);
    void setAlive(bool alive);
    void setActed(bool acted);

    //void render(Map map);

  private:
    Position position;
    char sprite;
    bool hasActed;
    bool isAlive;
};
