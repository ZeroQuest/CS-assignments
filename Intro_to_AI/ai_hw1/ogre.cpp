#include "ogre.h"
#include "actman.h"
#include <tuple>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

struct CompareFirstElement
{
  bool operator()(const std::tuple<int, int, int>& left, const std::tuple<int, int, int>& right) const
  {
    return std::get<0>(left) < std::get<0>(right);
  }
};

void Ogre::move(Map& map, Actman& actman)
{
  std::vector<std::tuple<int, int, int>>validMoveList;
  std::vector<std::tuple<int, int, int>>::iterator minIt = validMoveList.begin();
  int numPossibleMoves = 8;
  int newRow = getPosition().row;
  int newCol = getPosition().column;
  Position newPosition = {newRow, newCol};
  int dist = 0;
  int rowDiff = 0;
  int colDiff = 0;

  //std::cout<<"\t MONSTER MOVE FUNCTION!!!"<<std::endl;

  for(int i = 1; i < numPossibleMoves; i++)
  {
    newRow = getPosition().row;
    newCol = getPosition().column;
    switch (i) 
    {
      case 1:
        newRow--;
        break;
      case 2:
        newRow--;
        newCol++;
        break;
      case 3:
        newCol++;
        break;
      case 4:
        newRow++;
        newCol++;
        break;
      case 5:
        newRow++;
        break;
      case 6:
        newRow++;
        newCol--;
        break;
      case 7:
        newCol--;
        break;
      case 8:
        newRow--;
        newCol--;
        break;
    }
    //std::cout<<"\t\tAfter switch"<<std::endl;
    if(validMove(map, newRow, newCol))
    {
      rowDiff = (newRow - actman.getPosition().row);
      colDiff = (newCol - actman.getPosition().column);
      dist = (rowDiff * rowDiff) + (colDiff * colDiff);
      
      newPosition = {newRow, newCol};
      std::tuple<int, int, int> moveCandidate = std::make_tuple(dist, newPosition.row, newPosition.column);
      validMoveList.push_back(moveCandidate);
      //std::cout<<"\t\tEnd if!"<<std::endl;
    }
  }
 
  //std::cout<<"\t\tBefore min!"<<std::endl;

  minIt = std::min_element(validMoveList.begin(), validMoveList.end(), CompareFirstElement());

  newRow = std::get<1>(*minIt);
  newCol = std::get<2>(*minIt);

  //std::cout<<"\t\tNext Position: "<<newRow<<", "<<newCol<<std::endl;

  if(map.getCell(newRow, newCol) == static_cast<char>(Sprites::actman))
  {
    map.setSpriteAtCell(static_cast<char>(Sprites::space), getPosition().row, getPosition().column);
    actman.onDeath(map);

    std::cout<<"\t Ogre Killed Actman!"<<std::endl;
    setActed(true);
  }
  else if(!(map.getCell(newRow, newCol) == static_cast<char>(Sprites::space)) )
  {
    map.setSpriteAtCell(static_cast<char>(Sprites::space), getPosition().row, getPosition().column);
    setPosition(newRow, newCol);
    onDeath(map, actman);

    //std::cout<<"\t Moved into monster or corpse..."<<std::endl;
    setActed(true);
  }
  else if(map.getCell(newRow, newCol) == static_cast<char>(Sprites::space)) 
  {    
    map.setSpriteAtCell(static_cast<char>(Sprites::space), getPosition().row, getPosition().column);
    setPosition(newRow, newCol);
    map.setSpriteAtCell(static_cast<char>(Sprites::ogre), getPosition().row, getPosition().column);

    //std::cout<<"\t Normal Movement!"<<std::endl;
    setActed(true);
  }
}

void Ogre::onDeath(Map& map, Actman& actman)
{
  map.setSpriteAtCell(static_cast<char>(Sprites::corpse), getPosition().row, getPosition().column);
  setAlive(false);
  actman.addScore(5);
  //std::cout<<"\t\t\tOGRE DEAD!!!"<<std::endl;
}

void Ogre::canMove(Map& map, Actman& actman)
{
  if(map.getCell(getPosition().row, getPosition().column) == static_cast<char>(Sprites::corpse))
  {
    onDeath(map, actman);
  }
  else if(!actman.getAlive())
  {
    
  }
  else 
  {
    move(map, actman);
  }
}

