enum class Sprites : char
{
  space = ' ', wall = '#', actman = 'A', demon = 'D', ogre = 'G', corpse = '@', dead_actman = 'X'
};

enum class Direction : char
{
  southwest = '1', south = '2', southeast = '3', west = '4', 
  east = '6', northwest = '7', north = '8', northeast = '9'  
};

struct Position 
{
  Position(int row, int column)
    : row(row), column(column) {}
  int getRow() const {return row;}
  int getColumn() const {return column;}
  int row, column;
};

