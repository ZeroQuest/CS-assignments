#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <fstream>

struct Monster
{
  unsigned int row_pos = 0;
  unsigned int col_pos = 0;
  char sprite;

  Monster(char character, int row, int col) : sprite(character), row_pos(row), col_pos(col) {};
};

struct GameState
{
  unsigned int actman_row = 0;
  unsigned int actman_col = 0;
  //char actman_sprite = 'A';
  bool hasActmanShot = false;
  bool isActmanAlive = false;
  int score;
  std::vector<Monster> monster_list = {};
  std::string actionList = "";

  //unsigned int grid_row = 1;
  //unsigned int grid_col = 1;
  std::vector<std::vector<char>> grid;
};

//IO Functions
void readFromFile(const std::string& filename, GameState& state)
{
  std::string line;
  unsigned int row;
  unsigned int col;

  std::ifstream file(filename);
  if(!file.is_open())
    std::cerr << "Error: Unable to open file" << filename << std::endl;

  file >> row >> col;

  state.grid.resize(row, std::vector<char>(col));

  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  for(int i = 0; i < row; i++)
  {
    std::getline(file, line);
    for(int k = 0; k < col; k++)
    {
      if(line[k] == 'A')
      {
        state.actman_row = i;
        state.actman_col = k;
      }
      else if(line[k] == 'G')
        state.monster_list.push_back(Monster(i, k, 'G'));
      else if(line[k] == 'D')
        state.monster_list.push_back(Monster(i, k, 'D'));
        
      state.grid[i][k] = (line[k] == '#' || line[k] == ' ' || line[k] == '@') ? line[k] : ' ';
    }
  }
  file.close();
}

std::string writeToString(const GameState& state)
{
  std::string map;

  for(auto rowIt = state.grid.begin(); rowIt != state.grid.end(); ++rowIt)
  {
    for(auto colIt = rowIt->begin(); colIt != rowIt->end(); ++colIt)
    {
      map.push_back(*colIt);
    }
    map.push_back('\n');
  }
  return map;
}

//end Functions

int main(int argc, char* argv [])
{
  const std::string INPUT_FILENAME = argv[1];
  const std::string OUTPUT_FILENAME = argv[2];

  GameState state;

  std::string gameBoard;

  readFromFile(INPUT_FILENAME, state);

  gameBoard = writeToString(state);

  std::cout << gameBoard;

  std::ofstream file(OUTPUT_FILENAME);

  if(!file.is_open())
    std::cerr << "Error: Unable to open file." << std::endl;

  file << state.actionList << std::endl;
  file << state.score << std::endl;
  file << gameBoard;

  file.close();
  
  return 0;
}
