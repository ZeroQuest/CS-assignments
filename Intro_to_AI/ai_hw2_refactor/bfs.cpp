#include <cstdio>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>

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
  bool hasActmanShot = false;
  bool isActmanAlive = true;
  bool victory = false;
  int score = 50;
  std::vector<Monster> monster_list;
  std::string actionList = "";
  int stepNum = 0;

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

//Comparison Operator to compare valid moves
struct CompareFirstElement
{
  bool operator()(const std::tuple<int, int, int>& left, const std::tuple<int, int, int>& right) const
  {
    return std::get<0>(left) < std::get<0>(right);
  }
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
        state.monster_list.push_back(Monster('G', i, k));
      else if(line[k] == 'D')
        state.monster_list.push_back(Monster('D', i, k));
        
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
bool validMove(char action, const unsigned int& row, const unsigned int& col, const GameState& state)
{
  ActionMap actions = createActionMap();

  int newRow = row;
  int newCol = col;

  auto it = actions.find(action);

  if(it != actions.end() && (action >= '1' && action <= '9'))
  {
    newRow += it->second.first;
    newCol += it->second.second;

    if(!(state.grid[newRow][newCol] == '#'))
    {
      //std::cout << "Valid move!" << std::endl;
      return true;
    }
    else
    {
      //std::cout << "Invalid move!" << std::endl;
      return false;
    }
  }
  else if(it != actions.end() && (action == 'N' || action == 'S' | action == 'E' | action == 'W') && state.hasActmanShot == false)
  {
    //std::cout << "Valid Move!" << std::endl;
    return true;
  }
  else
  {
    //std::cout << "Invalid Move!" << std::endl;
    return false;
  }

  //return false;
}

void actmanShoot(GameState& state, ActionMap& actions, char direction)
{
  int currentRow = state.actman_row;
  int currentCol = state.actman_col;
  bool missed = false;

  std::vector<Monster> newList = {};

  auto it = actions.find(direction);

  while(it != actions.end() && state.grid[currentRow][currentCol] != '#')
  {
    

    if(state.grid[currentRow][currentCol] == 'G' || state.grid[currentRow][currentCol] == 'D')
    {
      missed = false;

      for(auto monsterIt = state.monster_list.begin(); monsterIt != state.monster_list.end(); ++monsterIt)
      {
          //std::cout << "Current Monster at: " << monsterIt->row_pos << ", " << monsterIt->col_pos << std::endl << "Bullet at: " << currentRow << ", " << currentCol << std::endl;
        if(monsterIt->row_pos == currentRow && monsterIt->col_pos == currentCol)
        {
          //std::cout << "Killed Monster at: " << monsterIt->row_pos << ", " << monsterIt->col_pos << std::endl;

          state.grid[currentRow][currentCol] = '@';
          //removalList.push_back(*monsterIt);
          state.score += 5;
        }
        else {
          //std::cout << "Monster Queue is likely too big... | " << monsterIt->sprite << std::endl; 
          newList.push_back(*monsterIt); 
        }
      }//end while
    }
    else {
      missed = true;
    }
    currentRow += it->second.first;
    currentCol += it->second.second;
  }//end while

  //Cleanup monster_list
  if(!missed)
    state.monster_list = newList;

  state.hasActmanShot = true;
  state.score = state.score - 20;
}

//Input Actions for testing!
void actmanInputMove(GameState& state, char input)
{
  ActionMap actions = createActionMap();

  int newRow = state.actman_row;
  int newCol = state.actman_col;

  auto it = actions.find(input);

  if(validMove(input, newRow, newCol, state))
  {
    if(it != actions.end() && (input >= '1' && input <= '9'))
    {
      newRow += it->second.first;
      newCol += it->second.second;
    }

    if(state.grid[newRow][newCol] == 'G' || state.grid[newRow][newCol] == 'D' || state.grid[newRow][newCol] == '@')
    {
      state.isActmanAlive = false;
      state.grid[state.actman_row][state.actman_col] = ' ';
      state.grid[newRow][newCol] = 'X';
      state.actman_row = newRow;
      state.actman_col = newCol;
      state.score = 0;
      state.actionList.push_back(it->first);
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

void monsterMove(GameState& state)
{
  ActionMap actions = createActionMap();
  
  std::vector<char> ogreOrder = {'8','9','6','3','2','1','4','7'};
  std::vector<char> demonOrder = {'8','7','4','1','2','3','6','9'};
  std::vector<Monster> newList;

  std::vector<std::tuple<int, int, int>> validMoveList;
  auto minIt = validMoveList.begin();
  //auto monsterIt = state.monster_list.begin();
  auto actionIt = actions.begin();

  unsigned int newRow = 0;
  unsigned int newCol = 0;
  int rowDiff = 0;
  int colDiff = 0;
  int dist = 0;

  //std::cout << "\t \t Outside Monster Turn Order!" << std::endl;

  for(auto monsterIt = state.monster_list.begin(); monsterIt != state.monster_list.end(); ++monsterIt)
  {
    newRow = monsterIt->row_pos;
    newCol = monsterIt->col_pos;

    //std::cout << "\t \t Running Monster Turn Order!" << std::endl;
    if(monsterIt->sprite == 'G')
    {
      //std::cout << "\t \t Generating Valid Ogre Moves!" << std::endl;
      for(auto it = ogreOrder.begin(); it != ogreOrder.end(); ++it)
      //for(int i = 0; i < ogreOrder.size(); i++)
      {
        actionIt = actions.find(*it);
        //actionIt = actions.find(ogreOrder[i]);

        if(actionIt != actions.end())
        {
          //std::cout << "\t\t\t Before new position!" << std::endl;
          
          newRow = monsterIt->row_pos;
          newCol = monsterIt->col_pos;
          
          
          if(validMove(actionIt->first, newRow, newCol, state))
          {
            newRow += actionIt->second.first;
            newCol += actionIt->second.second;
            
            //std::cout << "\t\t\t After new position!" << std::endl;
            //std::cout << "\t\t\t\t New Row: " << newRow << " | New Col: " << newCol << std::endl;
            
            rowDiff = (newRow - state.actman_row);
            colDiff = (newCol - state.actman_col);
            dist = (rowDiff * rowDiff) + (colDiff * colDiff);

            //std::cout << "\t\t\t Candidate move!" << std::endl;
            std::tuple<int, int, int> moveCandidate = std::make_tuple(dist, newRow, newCol);

            validMoveList.push_back(moveCandidate);
          }
        }
      }//end ogreOrder loop
    } //end ogre selection if
    else if(monsterIt->sprite == 'D')
    {
      //std::cout << "\t \t Generating Valid Demon Moves!" << std::endl;
      for(auto it = demonOrder.begin(); it != demonOrder.end(); ++it)
      {
        actionIt = actions.find(*it);
       
        if(actionIt != actions.end())
        {
          newRow = monsterIt->row_pos;
          newCol = monsterIt->col_pos;

          //std::cout << "\t\t\t Before new position!" << std::endl;
          //std::cout << "\t\t\t\t Start Row: " << newRow << " | Start Col: " << newCol << std::endl; 
          
          
          if(validMove(actionIt->first, newRow, newCol, state))
          {
            newRow += actionIt->second.first;
            newCol += actionIt->second.second;
            
            //std::cout << "\t\t\t After new position!" << std::endl;
            //std::cout << "\t\t\t\t New Row: " << newRow << " | New Col: " << newCol << std::endl;
            
            rowDiff = (newRow - state.actman_row);
            colDiff = (newCol - state.actman_col);
            dist = (rowDiff * rowDiff) + (colDiff * colDiff);

            //std::cout << "\t\t\t Candidate move!" << std::endl;
            std::tuple<int, int, int> moveCandidate = std::make_tuple(dist, newRow, newCol);
            //std::cout << "\t\t\t\t " << dist << ", " << newRow << ", " << newCol << std::endl;

            validMoveList.push_back(moveCandidate);
          }
        }
      }
    }//end demon selection if

    minIt = std::min_element(validMoveList.begin(), validMoveList.end(), CompareFirstElement());

    newRow = std::get<1>(*minIt);
    newCol = std::get<2>(*minIt);

    state.grid[monsterIt->row_pos][monsterIt->col_pos] = ' ';
    monsterIt->row_pos = newRow;
    monsterIt->col_pos = newCol;

    if(state.grid[newRow][newCol] == '@')
    {
      //std::cout << "\t\t Monster Moved Onto a Corpse and Died!" << std::endl;
      state.grid[newRow][newCol] = '@';
      //monsterIt->isAlive = false;
      state.score += 5;
    }
    else 
    {
      //std::cout << "\t\t Monster Moved!" << std::endl;
      state.grid[newRow][newCol] = monsterIt->sprite;
      newList.push_back(*monsterIt);
    }

    validMoveList.clear();
  }//end monster_list iteration
  state.monster_list = newList; 
}

bool checkVictory(GameState& state)
{
  //std::cout << "Are there monsters?" << state.monster_list.empty() << std::endl;

  if(state.monster_list.empty())
  {
    return state.victory = true;
  }
  else {
    return state.victory = false;
  }
}

void checkScore(GameState& state)
{
  if(state.score <= 0)
  {
    state.grid[state.actman_row][state.actman_col] = 'X';
    state.isActmanAlive = false;
  }
}

//Checks for monster overlapping with monsters and actman
void checkBoard(GameState& state)
{
  std::vector<Monster> newList = state.monster_list;
  auto nextIt = state.monster_list.begin();

  if(checkVictory(state))
    return;

  for(auto it = state.monster_list.begin(); it != state.monster_list.end(); ++it)
  {
    std::vector<unsigned int> indicesToRemove;

    for(unsigned int i = 0; i < state.monster_list.size(); ++i)
    {
      if(i != std::distance(state.monster_list.begin(), it) && 
         it->row_pos == state.monster_list[i].row_pos &&
         it->col_pos == state.monster_list[i].col_pos)
      {
        indicesToRemove.push_back(i);
      }
    }

    for(auto index = indicesToRemove.rbegin(); index != indicesToRemove.rend(); ++index)
    //for(auto index : indicesToRemove)
    {
      state.grid[it->row_pos][it->col_pos] = '@';
      state.score += 5;
      newList.erase(newList.begin() + *index);
    }
    

    //std::cout << "Monster 1: " << it->sprite << ", " << it->row_pos << ", " << it->col_pos << std::endl;
    
    //std::cout << "Monster 2: " << (it+1)->sprite << ", " << (it+1)->row_pos << ", " << (it+1)->col_pos << std::endl;
    
    if(it->row_pos == state.actman_row && it->col_pos == state.actman_col)
    {
      state.grid[state.actman_row][state.actman_col] = 'X';
      state.isActmanAlive = false;
      state.score = 0;
    }
  }
  state.monster_list = newList;
}

GameState transition(GameState state, char action)
{
  GameState newState = state;
  newState.stepNum += 1;
  std::string gameBoard;
  

  if((newState.isActmanAlive) && (newState.stepNum < 7) && !(newState.victory))
  {
    //std::cout << "Waiting for Input" << std::endl;
    if(validMove(action, newState.actman_row, newState.actman_col, newState))
    {
      actmanInputMove(newState, action);
    }
    else {
      newState.isActmanAlive = false;
      return newState;
    }

    //actmanInputMove(state, action);

    checkBoard(newState);

    if(newState.isActmanAlive)
      monsterMove(newState);

    checkBoard(newState);

    gameBoard = writeToString(newState);

    std::cout << newState.monster_list.size() << std::endl;

    std::cout << newState.actionList << std::endl;
    std::cout << gameBoard << std::endl;

    return newState;
  }
  //Invalid state shenanigans
  else 
  {
    newState.isActmanAlive = false;
    return newState;
  }
}

std::string breadthFirstSearch(GameState& state)
{
  GameState currentState = state;
  GameState nextState;
  std::string result;
  std::queue<std::string> frontier;
  std::vector<char> inputList = {'1','2','3','4','5','6','7','8','9','N','S','E','W'};
  std::unordered_map<std::string, GameState> visited;

  bool valid = false; 

  std::cout << "Start BFS" << std::endl;

  //populate queue with initial actions
  for(auto it = inputList.begin(); it != inputList.end(); ++it)
  {
    std::string initialAction = "";
    initialAction.push_back(*it);
    frontier.push(initialAction);
    visited[""] = state;
  }

  while(!frontier.empty())
  {
    std::string current = frontier.front();
    frontier.pop();

    std::cout << "\tTrying " << current << std::endl;

    std::string prefix = current;
    prefix.pop_back();


    auto it = visited.find(prefix);

    std::cout << "\tAfter prefix" << std::endl;
    //std::cout << "Trying to get key: " << it->first << std::endl;

    if(it == visited.end())
    {

    }
    else if(it->first == prefix)
    {
      std::cout << "\t\tFound previous state!" << std::endl;
      std::cout << "\t\tPrefix: " << prefix << std::endl;
      std::cout << "\t\tCurrent: " << current << std::endl;
      currentState = it->second;
    }

    if(validMove(current.back(), currentState.actman_row, currentState.actman_col, currentState))
    {
      nextState = transition(currentState, current.back());
      std::string next = nextState.actionList;
      visited[next] = nextState;
      
      std::cout << "\t" << current << " " << std::endl; 

      if(nextState.isActmanAlive == true && (nextState.victory == true || nextState.stepNum == 7))
      {
        std::cout << "\t VICTORY!" << std::endl;
        std::cout << "\t Victory: " << currentState.victory <<" Step #: " << currentState.stepNum << std::endl;
        std::cout << "\t Monster List Size: " << currentState.monster_list.size() << std::endl;
        result = next;
        break;
      }

      for(auto it = inputList.begin(); it != inputList.end(); ++it)
      {
        next = current + *it;
        frontier.push(next);
      }
    }
    else 
    {
      std::cout << "\t" << current << " is not valid." << std::endl;
    }
  }
  return result;
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

  int count = 0;
  char input;
  bool acted = false;

 /* for(auto it = state.monster_list.begin(); it != state.monster_list.end(); ++it)
    std::cout << "Monster: " << it->sprite << ", " << it->row_pos << ", " << it->col_pos << std::endl;

  std::cout << "Actman: " << state.actman_row << ", " << state.actman_col << std::endl;
*/
  std::cout << "Running" << std::endl;
  std::string final = breadthFirstSearch(state);
  
  for(auto it = final.begin(); it != final.end(); ++it)
  {
    state = transition(state, *it);
  }

  gameBoard = writeToString(state);

  std::cout << final << std::endl;
  std::cout << gameBoard;
  
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
