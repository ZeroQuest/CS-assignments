#include <algorithm>
#include <limits>
#include <ostream>
#include <tuple>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>

enum Graphics : char {  
  space = ' ', wall = '#', actman = 'A', 
  demon = 'D', ogre = 'G', corpse = '@', 
  dead_actman = 'X'
};

//Structs for Actors
struct Monster {
  //member vars
  unsigned int row_pos;
  unsigned int col_pos;
  bool isAlive = true;
  Graphics graphic;

  //Monster Functions
  Monster(unsigned int row, unsigned int col, Graphics character) : row_pos(row), col_pos(col), graphic(character) {}
};

struct GameState {
  //member vars
  unsigned int row_act;
  unsigned int col_act;
  int score = 50;
  bool isActmanAlive = true;
  bool hasActmanActed = false;
  bool hasActmanShot = false;
  Graphics graphic_act = Graphics::actman;

  unsigned int grid_col = 1;
  unsigned int grid_row = 1;
  std::vector<std::vector<char>> grid;

  std::vector<Monster> monsterList;

  std::string action;
  std::string actionList;

  unsigned int stepNum = 0;
  bool victory = false;
  bool visted = false;
};

//Maps
//Move Map
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
//Function Definitions

//File functions
void readFromFile(const std::string& filename, GameState& state);

void writeToFile(const std::string& filename, const GameState& state);

//Print the map to terminal
void printGrid(const GameState& state);

//Helper Functions
bool validMove(const unsigned int& row, const unsigned int& col, const GameState& state);

//Actman Functions
void actmanInputAction(GameState& state, char input);
void actmanShoot(GameState& state, ActionMap& actions, char direction);

//Monster functions
void monsterMove(GameState& state);

//Checking for corpse and actman's death
void checkBoard(GameState& state);

//steps 1 gameturn
void runTurn(GameState& state, char action);

//Runs a sequence of gameturns
void runStateSequence(GameState& state, std::string action);

//BFS Functions

//Performs BFS
std::string breadthFirstSearch(GameState& state);

//BFS transition functions
//Returns a sequence of actions taken
std::string transition(GameState state, std::string action);

//BFS goal Function 
//Returns if the goal has been achieved
bool goal(GameState state, std::string action);

int main(int argc, char* argv[]) 
{
  //File information
  const std::string INPUT_FILENAME = argv[1];
  const std::string OUTPUT_FILENAME = argv[2];

  GameState state;

  //Game Loop information
  unsigned int count = 0;

  //Read input
  readFromFile(INPUT_FILENAME, state);

  //Gather Actors
  for(int i = 1; i < state.grid_row - 1; i++)
  {
    for(int k = 1; k < state.grid_col - 1; k++)
    {
      if (state.grid[i][k] == Graphics::actman)
      {
        state.row_act = i;
        state.col_act = k;
        state.graphic_act = Graphics::actman;
      }
      else if(state.grid[i][k] == Graphics::ogre)
      {
        state.monsterList.push_back(Monster(i, k, Graphics::ogre));
      } 
      else if(state.grid[i][k] == Graphics::demon)
      {
        state.monsterList.push_back(Monster(i, k, Graphics::demon));
      }
    }
  }

  //Start Breadth First Search!

  std::cout << "Starting BFS!" << std::endl;

  std::string result;
  result = breadthFirstSearch(state);
  
  runStateSequence(state, result);   
  std::cout << "Final Results!" << std::endl << std::endl;
  std::cout << state.action << std::endl;
  std::cout << state.score << std::endl;
  printGrid(state);

  std::cout << "Write to file!" << std::endl;

  writeToFile(OUTPUT_FILENAME, state);

  return 0;
}

void readFromFile(const std::string& filename, GameState& state)
{
  std::string line;

  std::ifstream file(filename);
  if(!file.is_open())
  {
    std::cerr << "Error: Unable to open file " << filename << std::endl;
  }

  file >> state.grid_row >> state.grid_col;

  state.grid.resize(state.grid_row, std::vector<char>(state.grid_col));

  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  for(int i = 0; i < state.grid_row; i++)
  {
    std::getline(file, line);
    for(int k = 0; k < state.grid_col && k < line.length(); k++)
    {
      state.grid[i][k] = line[k];
    }
  }

  file.close();
}

void writeToFile(const std::string& filename, const GameState& state)
{
  std::ofstream file(filename);

  if(!file.is_open())
  {
    std::cerr << "Error: Unable to open file " << std::endl;
  }

  file<<state.action<<std::endl;
  file<<state.score<<std::endl;

  for(int i = 0; i < state.grid_row; i++)
  {
    for(int k = 0; k < state.grid_col; k++)
    {
      file << state.grid[i][k];
    }
    file<<std::endl;
  }

  file.close();
}

void printGrid(const GameState& state)
{
  for(int i = 0; i < state.grid_row; ++i)
  {
    for(int k = 0; k < state.grid_col; ++k)
    {
      std::cout<<state.grid[i][k];
    }
    std::cout<<std::endl;
  }
}

bool validMove(const unsigned int& row, const unsigned int& col, const GameState& state)
{
  if(!(state.grid[row][col] == Graphics::wall))
  {
    return true;
  }
  else 
    return false;
}

void actmanInputAction(GameState& state, char input)
{
  ActionMap actions = createActionMap();

  int newRow = state.row_act;
  int newCol = state.col_act;

  if(state.hasActmanShot == true)
  {
    actions.erase('N');
    actions.erase('S');
    actions.erase('E');
    actions.erase('W');
  }

  auto it = actions.find(input);

  if(it != actions.end() && (input >= '1' && input <= '9'))
  {

    newRow += it->second.first;
    newCol += it->second.second;

    //If actman moves onto a monster
    if((state.grid[newRow][newCol] == Graphics::ogre || state.grid[newRow][newCol] == Graphics::demon) 
      && validMove(newRow, newCol, state))
    {
      //std::cout << "\t Actman moved onto a monster and died!" << std::endl;

      state.isActmanAlive = false;
      state.grid[state.row_act][state.col_act] = Graphics::space;
      state.grid[newRow][newCol] = Graphics::dead_actman;
      state.row_act = newRow;
      state.col_act = newCol;
      state.hasActmanActed = true;
      state.score = 0;
    }
    else if(!(validMove(newRow, newCol, state)))
    {
      //std::cout << "\t NOT VALID MOVE??" << std::endl;
    }
    else if(state.grid[newRow][newCol] == Graphics::corpse && validMove(newRow, newCol, state))
    {
      //std::cout << "\t Actman moved onto a corpse and died!" << std::endl;

      state.isActmanAlive = false;
      state.grid[newRow][newCol] = Graphics::dead_actman;
      state.grid[state.row_act][state.col_act] = Graphics::space;
      state.row_act = newRow;
      state.col_act = newCol;
      state.hasActmanActed = true;
      state.score = 0;
    }
    //If actman moves to an empty space
    else if(validMove(newRow, newCol, state))
    {
      //std::cout << "\t Actman moved!" << std::endl;
      //std::cout << "\t\t Stats: " << std::endl;

      //std::cout << "\t\t Old Position: " << state.row_act << ", " << state.col_act << std::endl;

      state.grid[newRow][newCol] = Graphics::actman;
      state.grid[state.row_act][state.col_act] = Graphics::space;
      state.row_act = newRow;
      state.col_act = newCol;
      state.hasActmanActed = true;
      state.score = state.score - 1;

      //std::cout << "\t\t New Position: " << state.row_act << ", " << state.col_act << std::endl;
      //std::cout << "\t\t Graphic: " << state.grid[newRow][newCol] << std::endl;
    }
    state.actionList.push_back(it->first);
  }
  else if (it != actions.end() && (input == 'N' || input == 'S' || 
    input == 'E' || input == 'W'))
  {
    //std::cout << "\t Actman Shot!" << std::endl;

    actmanShoot(state, actions, input);
    state.actionList.push_back(it->first);
  }
  else
  {
    std::cout << "Invalid input!" << std::endl;
  }
}

void actmanShoot(GameState& state, ActionMap& actions, char direction)
{
  int currentRow = state.row_act;
  int currentCol = state.col_act;

  auto it = actions.find(direction);
  
  while (it != actions.end() && state.grid[currentRow][currentCol] != Graphics::wall) {
    
    if(state.grid[currentRow][currentCol] == Graphics::ogre || 
      state.grid[currentRow][currentCol] == Graphics::demon)
    {
      auto monsterIt = state.monsterList.begin();

      while(monsterIt != state.monsterList.end()) 
      {

        if(monsterIt->row_pos == currentRow && monsterIt->col_pos == currentCol)
        {
          state.grid[currentRow][currentCol] = Graphics::corpse;
          monsterIt->isAlive = false;
          //state.score = state.score + 5;
        }
        ++monsterIt;
      }
    }
    currentRow += it->second.first;
    currentCol += it->second.second;
  }

  state.hasActmanShot = true;
  state.hasActmanActed = true;
  state.score = state.score - 20;
}

void monsterMove(GameState& state)
{
  ActionMap actions = createActionMap();

  std::vector<std::tuple<int, int, int>> validMoveList;
  auto minIt = validMoveList.begin();
  auto monsterIt = state.monsterList.begin();
  auto actionIt = actions.begin();
  unsigned int newRow = 0;
  unsigned int newCol = 0;
  int rowDiff = 0;
  int colDiff = 0;
  int dist = 0;

  //std::cout << "\t \t Outside Monster Turn Order!" << std::endl;

  //monster turn order loop
  while(monsterIt != state.monsterList.end())
  {
    newRow = monsterIt->row_pos;
    newCol = monsterIt->col_pos;

    //std::cout << "\t \t Running Monster Turn Order!" << std::endl;

    //generate valid actions loop
    
    if(monsterIt->graphic == Graphics::ogre)
    {
      std::vector<char> ogreOrder = {'8','9','6','3','2','1','4','7'};

      //std::cout << "\t \t Generating Valid Ogre Moves!" << std::endl;

      for(int i = 0; i < ogreOrder.size(); i++)
      {
        actionIt = actions.find(ogreOrder[i]);

        //std::cout << "\t\t\t Move #" << i << std::endl; 

        if(actionIt != actions.end())
        {
          //std::cout << "\t\t\t Before new position!" << std::endl;
          
          newRow = monsterIt->row_pos;
          newCol = monsterIt->col_pos;

          newRow += actionIt->second.first;
          newCol += actionIt->second.second;

          //std::cout << "\t\t\t After new position!" << std::endl;
          //std::cout << "\t\t\t\t New Row: " << newRow << " | New Col: " << newCol << std::endl;

          if(validMove(newRow, newCol, state))
          {
            rowDiff = (newRow - state.row_act);
            colDiff = (newCol - state.col_act);
            dist = (rowDiff * rowDiff) + (colDiff * colDiff);

            //std::cout << "\t\t\t Candidate move!" << std::endl;

            std::tuple<int, int, int> moveCandidate = std::make_tuple(dist, newRow, newCol);
            validMoveList.push_back(moveCandidate);
          }
          //++actionIt;
        }
      }
    }
    else if(monsterIt->graphic == Graphics::demon)
    {
      std::vector<char> demonOrder = {'8','7','4','1','2','3','6','9'};

      //std::cout << "\t \t Generating Valid Demon Moves!" << std::endl;

      for(int i = 0; i < demonOrder.size(); i++)
      {
        actionIt = actions.find(demonOrder[i]);

        //std::cout << "\t\t\t Move #" << i << std::endl; 

        if(actionIt != actions.end())
        {
          newRow = monsterIt->row_pos;
          newCol = monsterIt->col_pos;
          
          //std::cout << "\t\t\t Before new position!" << std::endl;
          //std::cout << "\t\t\t\t Start Row: " << newRow << " | Start Col: " << newCol << std::endl; 
          
          newRow += actionIt->second.first;
          newCol += actionIt->second.second;

          //std::cout << "\t\t\t After new position!" << std::endl;
          //std::cout << "\t\t\t\t New Row: " << newRow << " | New Col: " << newCol << std::endl;


          if(validMove(newRow, newCol, state))
          {
            rowDiff = (newRow - state.row_act);
            colDiff = (newCol - state.col_act);
            dist = (rowDiff * rowDiff) + (colDiff * colDiff);

            //std::cout << "\t\t\t Candidate move!" << std::endl;
            
            std::tuple<int, int, int> moveCandidate = std::make_tuple(dist, newRow, newCol);
            validMoveList.push_back(moveCandidate);
          }
          //++actionIt;
        }
      }
    }

    if(validMoveList.empty())
      std::cout << "\t\t\t Move List Empty???" << std::endl;
    minIt = std::min_element(validMoveList.begin(), validMoveList.end(), CompareFirstElement());

    newRow = std::get<1>(*minIt);
    newCol = std::get<2>(*minIt);

    state.grid[monsterIt->row_pos][monsterIt->col_pos] = Graphics::space;
    monsterIt->row_pos = newRow;
    monsterIt->col_pos = newCol;

    if(state.grid[newRow][newCol] == Graphics::corpse)
    {
      //std::cout << "\t\t Monster Moved Onto a Corpse and Died!" << std::endl;

      state.grid[newRow][newCol] = Graphics::corpse;
      monsterIt->isAlive = false;
    }
    else
    {
      //std::cout << "\t\t Monster Moved!" << std::endl;
      state.grid[newRow][newCol] = monsterIt->graphic;
    }
    
    validMoveList.clear();
    ++monsterIt;
  }
}


void checkBoard(GameState& state)
{
  //std::vector<Monster> removalList;
  auto monsterIt = state.monsterList.begin();
  

  if(state.monsterList.empty())
  {
    //VICTORY!
    state.victory = true;
    return;
  }

  for(auto it = state.monsterList.begin(); it != state.monsterList.end(); ++it)
  {
    for(auto compareMonsterIt = it + 1; compareMonsterIt != state.monsterList.end(); ++compareMonsterIt)
    {
      if(compareMonsterIt->row_pos == monsterIt->row_pos && 
        compareMonsterIt->col_pos == monsterIt->col_pos)
      {
        compareMonsterIt->isAlive = false;
        monsterIt->isAlive = false;
      }
    }

    if(it->row_pos == state.row_act && it->col_pos == state.col_act)
    {
      //std::cout << "\t Monster on top of Actman!" << std::endl;

      state.graphic_act = Graphics::dead_actman;
      state.grid[state.row_act][state.col_act] = state.graphic_act;
      state.isActmanAlive = false;
      state.score = 0;
    }
  }

  for(auto it = state.monsterList.begin(); it != state.monsterList.end(); ++it)
  {
    if(it->isAlive == false)
      state.score = state.score + 5;
  }

  state.monsterList.erase(std::remove_if(state.monsterList.begin(), state.monsterList.end(), [](const Monster& monster) {return !monster.isAlive; }), state.monsterList.end());
}

std::string transition(GameState state, std::string action)
{
  GameState transitionState = state;

  std::cout << std::endl << "Initial State!" << std::endl << std::endl;
  printGrid(transitionState);
  
  for(auto it = action.begin(); it != action.end(); ++it)
  {
    runTurn(transitionState, *it);

    //std::cout << "State Step: " << transitionState.stepNum << " Action: " << *it << std::endl;
    //printGrid(transitionState);
  }

  std::cout << "After actions!" << std::endl;
  printGrid(transitionState);

  return transitionState.actionList;
}

bool goal(GameState state, std::string action)
{
  GameState goalState = state;

  for(auto it = action.begin(); it != action.end(); ++it)
  {
    runTurn(goalState, *it); 
  }
  if(goalState.stepNum +1 == 7 || goalState.victory)
  {
    return true;
  }
  else {
    return false;
  }
}

void runStateSequence(GameState& state, std::string action)
{
  for(auto it = action.begin(); it != action.end(); ++it)
  {
    runTurn(state, *it); 
  }
  state.action = action;
}

void runTurn(GameState& state, char action)
{
  state.action += action;
  //Game Loop
    if((state.isActmanAlive) && state.stepNum < state.action.size() && !(state.victory)) 
    {
      //std::cout << "\t Step Number: " << state.stepNum << std::endl;

      //Actman turn!
      actmanInputAction(state, action);
      
      //std::cout << "\t Actman New Position: " << state.row_act << ", " << state.col_act << std::endl;

      //Actman death check!
      checkBoard(state);

      //Monster turn!
      if(state.isActmanAlive == true && state.hasActmanActed == true)
        monsterMove(state);

      //Corpse Check!
      checkBoard(state);

      //Actman death check!

      //Score check!
      if(state.score <= 0)
      {
        state.isActmanAlive = false;
        state.grid[state.row_act][state.col_act] = Graphics::dead_actman;
      }

      state.hasActmanActed = false;
      state.stepNum++;
    }//End Game Loop
    
    //if(state.victory)
      //std::cout << "==============================[ VICTORY! ]===============================" << std::endl;
}

std::string breadthFirstSearch(GameState& state)
{
  std::string result;
  std::queue<std::string> frontier; 
  std::vector<char> inputList = {'1','2','3','4','5','6','7','8','9','N','S','E','W'};

  std::string current = "";
  frontier.push(current);

  while(!frontier.empty())
  {

    std::string current = frontier.front();
    frontier.pop();

    std::cout << "Before Transistion! " << current << std::endl;

    std::string next = transition(state, current);

    if(goal(state, current))
    {
      std::cout << "Found Goal!" << std::endl;
      result = next;
      break;
    }

    for(auto it = inputList.begin(); it != inputList.end(); ++it)
    {
      //std::cout << "Putting in new elements!" << std::endl;

      std::string next = current + *it;
      frontier.push(next);
    }
  }
  return result;
}
