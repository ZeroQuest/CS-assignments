#include <cstdio>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <fstream>
#include <algorithm>

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
  bool victory = false;
  int score;
  std::vector<Monster> monster_list = {};
  std::string actionList = "";

  //unsigned int grid_row = 1;
  //unsigned int grid_col = 1;
  std::vector<std::vector<char>> grid;
};

using ActionMap = std::unordered_map<char, std::pair<int, int>>;

ActionMap createActionMap()
{
  ActionMap actions;

  actions['1'] = std::make_pair(1, -1);
  actions['2'] = std::make_pair(1, 0);
  actions['3'] = std::make_pair(1, 1);
  actions['4'] = std::make_pair(0, -1);
  actions['6'] = std::make_pair(0, 1);
  actions['7'] = std::make_pair(-1, -1);
  actions['8'] = std::make_pair(-1, 0);
  actions['9'] = std::make_pair(-1, 1);
  actions['N'] = std::make_pair(-1, 0);
  actions['S'] = std::make_pair(1, 0);
  actions['E'] = std::make_pair(0, 1);
  actions['W'] = std::make_pair(0, -1);

  return actions;
}

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
        
      //state.grid[i][k] = (line[k] == '#' || line[k] == ' ' || line[k] == '@') ? line[k] : ' ';
      state.grid[i][k] = line[k];
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

//Move Validation
bool validMove(char action, const GameState& state)
{
  ActionMap actions = createActionMap();

  int newRow = state.actman_row;
  int newCol = state.actman_col;

  auto it = actions.find(action);

  if(it != actions.end() && (action >= '1' && action <= '9'))
  {
    newRow += it->second.first;
    newCol += it->second.second;

    if(!(state.grid[newRow][newCol] == '#'))
    {
      std::cout << "Valid move!" << std::endl;
      return true;
    }
    else
    {
      std::cout << "Invalid move!" << std::endl;
      return false;
    }
  }
  else if(it != actions.end() && (action == 'N' || action == 'S' | action == 'E' | action == 'W') && state.hasActmanShot == false)
  {
    std::cout << "Valid Move!" << std::endl;
    return true;
  }
  else
  {
    std::cout << "Invalid Move!" << std::endl;
    return false;
  }

  //return false;
}

void actmanShoot(GameState& state, ActionMap& actions, char direction)
{
  int currentRow = state.actman_row;
  int currentCol = state.actman_col;

  std::vector<Monster> newList = {};

  auto it = actions.find(direction);

  while(it != actions.end() && state.grid[currentRow][currentCol] != '#')
  {
    if(state.grid[currentRow][currentCol] == 'G' || state.grid[currentRow][currentCol] == 'D')
    {
      for(auto monsterIt = state.monster_list.begin(); monsterIt != state.monster_list.end(); ++monsterIt)
      {
        if(monsterIt->row_pos == currentRow && monsterIt->col_pos == currentCol)
        {
          state.grid[currentRow][currentCol] = '@';
          //removalList.push_back(*monsterIt);
          state.score += 5;
        }
        else {
          newList.push_back(*monsterIt); 
        }
      }//end while
    }
    currentRow += it->second.first;
    currentCol += it->second.second;
  }//end while

  //Cleanup monster_list
  state.monster_list = newList;

  state.hasActmanShot = true;
  state.score = state.score - 20;
}

//Input Actions for testing!
void actmanInputMove(GameState& state, char input)
{
  ActionMap actions = createActionMap();

  int newRow = 0;
  int newCol = 0;

  auto it = actions.find(input);

  if(validMove(input, state))
  {
    newRow = state.actman_row + it->second.first;
    newCol = state.actman_col + it->second.second;

    if(state.grid[newRow][newCol] == 'G' || state.grid[newRow][newCol] == 'D' || state.grid[newRow][newCol] == '@')
    {
      state.isActmanAlive = false;
      state.grid[state.actman_row][state.actman_col] = ' ';
      state.grid[newRow][newCol] = 'X';
      state.actman_row = newRow;
      state.actman_col = newCol;
      state.score = 0;
    }
    else if(input == 'N' || input == 'S' || input == 'E' || input == 'W')
    {
      actmanShoot(state, actions, input);
      state.actionList.push_back(it->first);
    }
    else 
    {
      state.grid[newRow][newCol] = 'A';
      state.grid[state.actman_row][state.actman_col] = ' ';
      state.actman_row = newRow;
      state.actman_col = newCol;
      state.score -= 1;
      state.actionList.push_back(it->first);
    }
  }
  else 
    std::cout << "INVALID INPUT" << std::endl;
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

  int count = 1;
  char input;
  bool acted = false;

  std::cout << "Running" << std::endl;

  while((state.isActmanAlive) && (count < 7) && !(state.victory))
  {
    std::cout << "Entered Loop" << std::endl;
    while(acted == false)
    {
      std::cout << "Waiting for Input" << std::endl;
      std::cin >> input;
      if(validMove(input, state))
      {
        acted = true;
      }
      else {
        input = ' ';
      }
    }

    actmanInputMove(state, input);

    gameBoard = writeToString(state);

    std::cout << gameBoard;
    
    acted = false;
    count++;
  }
  
  //Write to File
  std::ofstream file(OUTPUT_FILENAME);

  if(!file.is_open())
    std::cerr << "Error: Unable to open file." << std::endl;

  file << state.actionList << std::endl;
  file << state.score << std::endl;
  file << gameBoard;

  file.close();
  
  return 0;
}
