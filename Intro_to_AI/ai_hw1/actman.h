#pragma once
#include "entity.h"
#include "map.h"

class Actman : public Entity
{
  public:
    Actman(Position position) : Entity(position.getRow(), position.getColumn(), Sprites::actman){score = 50; hasBullet = true;};

    char action(Map& map);

    void move(Map& map);
    void onDeath(Map& map);

    void playerMove(Map& map, char input);
    void shoot(Map& map, Direction dir);
    
    int getScore();
    void setScore(int newscore);

    void addScore(int num);
    void subScore(int num);

  private:
    bool hasBullet;
    int score;
};
