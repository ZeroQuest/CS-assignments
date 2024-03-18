#include "actman.h"
#include <iostream>
#include <random>
#include "demon.h"
#include "ogre.h"

void Actman::move(Map& map)
{

}

char Actman::action(Map& map)
{
  std::random_device rand;
  std::mt19937 gen(rand());
  std::uniform_int_distribution<> distribution(1, 12);
  int randomAction = 0;
  char action = ' ';
 
  randomAction = distribution(gen);

  while(hasBullet == false && (randomAction == 12 || randomAction == 11 || randomAction == 10 || randomAction == 9 ))
  {
    randomAction = distribution(gen);
  }

  switch (randomAction) 
  {
    case 1:
      playerMove(map, static_cast<char>(Direction::southwest));
      action = static_cast<char>(Direction::southwest);
      break;
    case 2:
      playerMove(map, static_cast<char>(Direction::south));
      action = static_cast<char>(Direction::south);
      break;
    case 3:
      playerMove(map, static_cast<char>(Direction::southeast));
      action = static_cast<char>(Direction::southeast);
      break;
    case 4:
      playerMove(map, static_cast<char>(Direction::east));
      action = static_cast<char>(Direction::east);
      break;
    case 5:
      playerMove(map, static_cast<char>(Direction::west));
      action = static_cast<char>(Direction::west);
      break;
    case 6:
      playerMove(map, static_cast<char>(Direction::northwest));
      action = static_cast<char>(Direction::northwest);
      break;
    case 7:
      playerMove(map, static_cast<char>(Direction::north));
      action = static_cast<char>(Direction::north);
      break;
    case 8:
      playerMove(map, static_cast<char>(Direction::northeast));
      action = static_cast<char>(Direction::northeast);
      break;
    case 9:
      playerMove(map, 'N');
      action = 'N';
      break;
    case 10:
      playerMove(map, 'S');
      action = 'S';
      break;
    case 11:
      playerMove(map, 'E');
      action = 'E';
      break;
    case 12:
      playerMove(map, 'W');
      action = 'W';
      break;
  }
  return action;
 // map.setSpriteAtCell();
}

void Actman::onDeath(Map& map)
{
  map.setSpriteAtCell(static_cast<char>(Sprites::dead_actman), getPosition().row, getPosition().column);
  setScore(0);
  setAlive(false);
  setActed(true);
  std::cout<<"\t Ondeath: "<<getAlive()<<std::endl;
}

void Actman::playerMove(Map& map, char input)
{
  Position currentPos = getPosition();
  char currentSprite = getSprite();
  int newRow = currentPos.row;
  int newCol = currentPos.column;

  switch(input)
  {
    case '1':
      newRow++;
      newCol--;
      break;
    case '2':
      newRow++;
      break;
    case '3':
      newRow++;
      newCol++;
      break;
    case '4':
      newCol--;
      break;
    case '6':
      newCol++;
      break;
    case '7':
      newRow--;
      newCol--;
      break;
    case '8':
      newRow--;
      break;
    case '9':
      newRow--;
      newCol++;
      break;
    case 'N':
      if(hasBullet)
        shoot(map, Direction::north);
      break;
    case 'S':
      if(hasBullet)
        shoot(map, Direction::south);
      break;
    case 'E':
      if(hasBullet)
        shoot(map, Direction::east);
      break;
    case 'W':
      if(hasBullet)
        shoot(map, Direction::west);
      break;
    default:
      std::cout<<"Invalid input"<<std::endl;
      return;
  }

  if(input >= '1' && input <= '9')
  {
    if(!(map.getCell(newRow, newCol) == static_cast<char>(Sprites::space) || 
      map.getCell(newRow, newCol) == static_cast<char>(Sprites::actman)) 
      && validMove(map, newRow, newCol))
    {
      map.setSpriteAtCell(static_cast<char>(Sprites::space), 
        currentPos.row, currentPos.column);
      setPosition(newRow, newCol);
      setActed(true);
      subScore(1);
      onDeath(map);
    }
    else if(validMove(map, newRow, newCol))
    {
      map.setSpriteAtCell(currentSprite, newRow, newCol);
      map.setSpriteAtCell(static_cast<char>(Sprites::space), 
        currentPos.row, currentPos.column);
      setPosition(newRow, newCol);
      setActed(true);
      subScore(1);
    }
  }
  else
  {
    
  }

  /*if(!(map.getCell(newRow, newCol) == static_cast<char>(Sprites::space) || 
    map.getCell(newRow, newCol) == static_cast<char>(Sprites::actman)))
  {
    onDeath(map);
  }*/
}

void Actman::shoot(Map& map, Direction dir)
{
  Position currentPos = getPosition();
  int currentRow = currentPos.row;
  int currentCol = currentPos.column;
  char spriteInCell;

  switch(dir)
  {
    case Direction::north:
      currentRow = currentRow - 1;
      spriteInCell = map.getCell(currentRow, currentCol);

      while(spriteInCell != static_cast<char>(Sprites::wall))
      {
        if(spriteInCell == static_cast<char>(Sprites::ogre) || 
           spriteInCell == static_cast<char>(Sprites::demon))
        {
          map.setSpriteAtCell(static_cast<char>(Sprites::corpse), currentRow, currentCol);
        }

        currentRow--;
        spriteInCell = map.getCell(currentRow, currentCol);
      } // end while
      break;
    case Direction::south:
      currentRow = currentRow + 1;
      spriteInCell = map.getCell(currentRow, currentCol);

      while(spriteInCell != static_cast<char>(Sprites::wall))
      {
        if(spriteInCell == static_cast<char>(Sprites::ogre) || 
           spriteInCell == static_cast<char>(Sprites::demon))
        {
          map.setSpriteAtCell(static_cast<char>(Sprites::corpse), currentRow, currentCol);
        }

        currentRow++;
        spriteInCell = map.getCell(currentRow, currentCol);
      }
      break;
    case Direction::east:
      currentCol = currentCol + 1;
      spriteInCell = map.getCell(currentRow, currentCol);

      while(spriteInCell != static_cast<char>(Sprites::wall))
      {
        if(spriteInCell == static_cast<char>(Sprites::ogre) || 
           spriteInCell == static_cast<char>(Sprites::demon))
        {
          map.setSpriteAtCell(static_cast<char>(Sprites::corpse), currentRow, currentCol);
        }

        currentCol++;
        spriteInCell = map.getCell(currentRow, currentCol);
      }
      break;
    case Direction::west:
      currentCol = currentCol - 1;
      spriteInCell = map.getCell(currentRow, currentCol);

      while(spriteInCell != static_cast<char>(Sprites::wall))
      {
        if(spriteInCell == static_cast<char>(Sprites::ogre) || 
           spriteInCell == static_cast<char>(Sprites::demon))
        {
          map.setSpriteAtCell(static_cast<char>(Sprites::corpse), currentRow, currentCol);
        }

        currentRow--;
        spriteInCell = map.getCell(currentRow, currentCol);
      }
      break;
/*    case Direction::northwest:
      break;
    case Direction::northeast:
      break;
    case Direction::southeast:
      break;
    case Direction::southwest:
      break;*/
    default:
      break;
  }
  hasBullet = false;
  setActed(true);
  subScore(20);
}

int Actman::getScore()
{
  return score;
}

void Actman::setScore(int newscore)
{
  score = newscore;
}

void Actman::addScore(int num)
{
  score = score + num;
}

void Actman::subScore(int num)
{
  score = score - num;
  if(score <= 0)
  {
    setAlive(false);
  }
}
