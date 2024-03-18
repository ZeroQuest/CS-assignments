//Name: Timothy Stokes
//CS5400

#include "map.h"
#include <iterator>
#include <string>
#include <iostream>
#include <vector>
#include "actman.h"
#include "ogre.h"
#include "demon.h"

bool isOgreDead(Ogre& ogre);

bool isDemonDead(Demon& demon);

void monsterTurn(Map& map, Actman& actman, std::vector<Ogre>& ogreList, std::vector<Demon>& demonList);

void checkForActmanDeath(Map& map, Actman& actman, std::vector<Ogre>& ogreList, std::vector<Demon>& demonList);

void resetMonsterFlags(std::vector<Ogre>& ogreList, std::vector<Demon>& demonList);

int main (int argc, char* argv[])
{
  const std::string input_filename = argv[1];
  const std::string output_filename = argv[2];
  std::vector<Ogre> ogreList;
  std::vector<Demon> demonList;
  Position monsterPos = {-1, -1};

  Map map;

  map.readGridFromFile(input_filename);

  //map.printGrid();

  //Find actman
  Position actmanPos = {-1,-1};

  for(int i = 0; i < map.getRows(); i++)
  {
    for(int k = 0; k < map.getColumns(); k++)
    {
      if(map.getCell(i, k) == static_cast<char>(Sprites::actman))
      {
        actmanPos = {i, k};
      }
    }
  }

  //find monsters
  for(int i = 1; i < map.getRows() - 1; i++)
  {
    for(int k = 1; k < map.getColumns() - 1; k++)
    {
      if(map.getCell(i, k) == static_cast<char>(Sprites::ogre))
      {
        monsterPos = {i, k};
        ogreList.push_back(Ogre(monsterPos));
      } 
      else if(map.getCell(i, k) == static_cast<char>(Sprites::demon))
      {
        monsterPos = {i, k};
        demonList.push_back(Demon(monsterPos));
      }
    }
  }

/*  std::cout<<"Monster's in list: "<<std::endl;
  for(int i = 0; i < monsterList.size(); i++)
  {
    monsterPos = monsterList[i]->getPosition();
      std::cout<<"\t "<<monsterList[i]->getSprite()<<", ("<<monsterPos.row<<" ,"<<monsterPos.column<<")"<<std::endl;
  }
*/
  Actman actman(actmanPos);

  std::cout<<"Actman at: "<<actmanPos.row<<", "<<actmanPos.column<<std::endl;
  std::cout<<"Score: "<<actman.getScore()<<std::endl<<std::endl;
  std::cout<<"Initial State!"<<std::endl;
  map.printGrid();

  int count = 0;
  char action;
  std::vector<char> actionList;

  //Game Loop
  while(actman.getAlive() != false && (count < 7))
  {

    std::cout<<"Round #: "<<count+1<<std::endl;
    actman.setActed(false);
    //actman's turn
    while(!(actman.getActed()))
    {
      action = actman.action(map);
    }
    
    actmanPos = actman.getPosition();
    actionList.push_back(action);

    std::cout<<"Actman Turn!"<<std::endl;
    std::cout<<"Actman at: "<<actmanPos.row<<", "<<actmanPos.column<<std::endl;
    std::cout<<"Score: "<<actman.getScore()<<std::endl;
    std::cout<<"\tAction: "<<actionList[count]<<std::endl;

    //checkForActmanDeath(map, actman, ogreList, demonList);
    
/*    if(!actman.getAlive())
    {
      break;
    }
*/
    //Monster turn!
    std::cout<<"Monster Turn!"<<std::endl;

    monsterTurn(map, actman, ogreList, demonList);

    auto ogreIt = ogreList.begin();
    auto demonIt = demonList.begin();

    while(ogreIt != ogreList.end())
    {
      int ogreNum = std::distance(ogreList.begin(), ogreIt);
      std::cout<<"\tOgre: "<<ogreNum<<" At: ("<<ogreIt->getPosition().row<<", "<<ogreIt->getPosition().column<<") "<<std::endl;
      ++ogreIt;
    }
    while(demonIt != demonList.end())
    {
      int demonNum = std::distance(demonList.begin(), demonIt);
      std::cout<<"\tDemon: "<<demonNum<<" At: ("<<demonIt->getPosition().row<<", "<<demonIt->getPosition().column<<") "<<std::endl;
      ++demonIt;
    }
    
    
    std::cout<<"Score: "<<actman.getScore()<<std::endl;

    //Did they kill actman?
    //checkForActmanDeath(map, actman, ogreList, demonList);
    
/*    if(!actman.getAlive())
    {
      break;
    }
*/
    //reset monster's turn flags
    resetMonsterFlags(ogreList, demonList);

    map.printGrid();
    count++;
  }//end while

  std::cout<<std::endl<<"End State"<<std::endl;
  std::cout<<"Action List: ";
  std::string actions;

  for(int i = 0; i < count; i++)
  {
      actions += actionList[i];
  }
  std::cout<<actions<<std::endl;
  std::cout<<"Final Score: "<<actman.getScore()<<std::endl;
  map.printGrid();

  std::cout<<"Is actman alive: "<<actman.getAlive()<<std::endl;

  map.writeGridToFile(output_filename, actions, actman.getScore());

  return 0;
}

void monsterTurn(Map& map, Actman& actman, std::vector<Ogre>& ogreList, std::vector<Demon>& demonList)
{
  std::vector<Ogre>::iterator ogreIt = ogreList.begin();
  std::vector<Demon>::iterator demonIt = demonList.begin();

  while(ogreIt != ogreList.end())
  {
    if(!ogreIt->getActed() && ogreIt->getAlive())
    {
      ogreIt->canMove(map, actman);
    }
    ++ogreIt;
  }
  while(demonIt != demonList.end())
  {
    if(!demonIt->getActed() && demonIt->getAlive())
    {
      demonIt->canMove(map, actman);
    }
    ++demonIt;
  }
}

void checkForActmanDeath(Map& map, Actman& actman, std::vector<Ogre>& ogreList, std::vector<Demon>& demonList)
{
  std::vector<Ogre>::iterator ogreIt = ogreList.begin();
  std::vector<Demon>::iterator demonIt = demonList.begin();

  while(ogreIt != ogreList.end())
  {
    if(actman.getPosition().row == ogreIt->getPosition().row &&
      actman.getPosition().column == ogreIt->getPosition().column) 
    {
      actman.onDeath(map);
      break;
    }
    ++ogreIt;
  }
  while(demonIt != demonList.end())
  {
    if(actman.getPosition().row == demonIt->getPosition().row &&
      actman.getPosition().column == demonIt->getPosition().column)
    {
      actman.onDeath(map);
      break;
    }
    ++demonIt;
  }
}

void resetMonsterFlags(std::vector<Ogre>& ogreList, std::vector<Demon>& demonList)
{
  std::vector<Ogre>::iterator ogreIt = ogreList.begin();
  std::vector<Demon>::iterator demonIt = demonList.begin();

  while(ogreIt != ogreList.end())
  {
    if(ogreIt->getAlive())
    {
      ogreIt->setActed(false);
    }
    ++ogreIt;
  }
  //ogreList.erase(std::remove_if(ogreList.begin(), ogreList.end(), isOgreDead), ogreList.end());

  while(demonIt != demonList.end())
  {
    if(demonIt->getAlive())
    {
      demonIt->setActed(false);
    }
    ++demonIt;
  }
  //demonList.erase(std::remove_if(demonList.begin(), demonList.end(), isDemonDead), demonList.end());
}

