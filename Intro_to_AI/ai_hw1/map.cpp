#include "map.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

Map::Map()
  : rows(0), columns(0)
{
  grid = std::vector<std::vector<char>>(rows, std::vector<char>(columns, static_cast<char>(Sprites::space)));
}

Map::Map(int rows, int columns, char sprite)
  : rows(rows), columns(columns)
{
  grid = std::vector<std::vector<char>>(rows, std::vector<char>(columns, sprite));
}

Map::~Map()
{
  columns = 0;
  rows = 0;
  grid.clear();
  grid.shrink_to_fit();
}

void Map::setSpriteAtCell(char sprite, int row_pos, int col_pos)
{
  grid[row_pos][col_pos] = sprite;
}

char Map::getCell(int row_pos, int col_pos)
{
  return grid[row_pos][col_pos];
}

void Map::printGrid()
{
  for(int i = 0; i < rows; ++i)
  {
    for(int k = 0; k < columns; ++k)
    {
        std::cout<<grid[i][k];
    }
      std::cout<<std::endl;
  }
}

void Map::readGridFromFile(const std::string& filename)
{
  std::string line;

  std::ifstream file(filename);
  if(!file.is_open())
  {
    std::cerr << "Error: Unable to open file " << filename << std::endl;
  }

  file>>rows>>columns;

  grid.resize(rows, std::vector<char>(columns));

  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
 
  for(int i = 0; i < rows; i++)
  {
    std::getline(file, line);
    for(int k = 0; k < columns && k < line.length(); k++)
    {
      grid[i][k] = line[k];
    }
  }

  file.close();
}

void Map::writeGridToFile(const std::string& filename, const std::string& actions, const int& score)
{
  std::ofstream file(filename);

  if(!file.is_open())
  {
    std::cerr << "Error: Unable to open file " << std::endl;
  }

  file<<actions<<std::endl;
  file<<score<<std::endl;

  for(int i = 0; i < rows; i++)
  {
    for(int k = 0; k < columns; k++)
    {
      file << grid[i][k];
    }
    file<<std::endl;
  }

  file.close();
}
