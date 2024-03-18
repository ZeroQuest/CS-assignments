#pragma once
#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include "enums.h"

class Map 
{
  public:
    Map();
    Map(int rows, int columns, char sprite);
    ~Map();

    int getColumns() const {return columns;}
    int getRows() const {return rows;}

    void setColumns(int cols) {columns = cols;}
    void setRows(int rows) {rows = rows;}

    void setSpriteAtCell(char sprite, int row_pos, int col_pos);
    char getCell(int row_pos, int col_pos);

    void printGrid();

    void readGridFromFile(const std::string& filename);
    void writeGridToFile(const std::string& filename, const std::string& actions, const int& score);

  private:
    int columns;
    int rows;
    std::vector<std::vector<char>> grid; 
};

#endif
