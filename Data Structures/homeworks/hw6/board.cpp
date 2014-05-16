#include <cmath>
#include <iostream>
#include <iomanip>
#include "board.h"


// ==================================================================
// ==================================================================
// Implementation of the Position class


// allows a position to be output to a stream
std::ostream& operator<<(std::ostream &ostr, const Position &p) {
  ostr << '(' << p.row << "," << p.col << ')';
  return ostr;
}


// equality and inequality comparision of positions
bool operator==(const Position &a, const Position &b) {
  return (a.row == b.row && a.col == b.col);
}
bool operator!=(const Position &a, const Position &b) {
  return !(a==b);
}


// ==================================================================
// ==================================================================
// Implementation of the Board class


// ===================
// CONSTRUCTOR
// ===================

Board::Board(int r, int c) {
  // initialize the dimensions
  rows = r;
  cols = c;

  // allocate space for the contents of each grid cell
  board = std::vector<std::vector<char> >(rows,std::vector<char>(cols,' '));

  // allocate space for booleans indicating the presense of each wall
  // by default, these are false == no wall
  // (note that there must be an extra column of vertical walls
  //  and an extra row of horizontal walls)
  vertical_walls = std::vector<std::vector<bool> >(rows,std::vector<bool>(cols+1,false));
  horizontal_walls = std::vector<std::vector<bool> >(rows+1,std::vector<bool>(cols,false));

  // initialize the outermost edges of the grid to have walls
  for (int i = 0; i < rows; i++) {
    vertical_walls[i][0] = vertical_walls[i][cols] = true;
  }
  for (int i = 0; i < cols; i++) {
    horizontal_walls[0][i] = horizontal_walls[rows][i] = true;
  }
}


// ===================
// ACCESSORS related to board geometry
// ===================

// Query the existance of a horizontal wall
bool Board::getHorizontalWall(double r, int c) const {
  // verify that the requested wall is valid
  // the row coordinate must be a half unit
  assert (fabs((r - floor(r))-0.5) < 0.005);
  assert (r >= 0.4 && r <= rows+0.6);
  assert (c >= 1 && c <= cols);
  // subtract one and round down because the corner is (0,0) not (1,1)
  return horizontal_walls[floor(r)][c-1];
}

// Query the existance of a vertical wall
bool Board::getVerticalWall(int r, double c) const {
  // verify that the requested wall is valid
  // the column coordinate must be a half unit
  assert (fabs((c - floor(c))-0.5) < 0.005);
  assert (r >= 1 && r <= rows);
  assert (c >= 0.4 && c <= cols+0.6);
  // subtract one and round down because the corner is (0,0) not (1,1)
  return vertical_walls[r-1][floor(c)];
}


// ===================
// MODIFIERS related to board geometry
// ===================

// Add an interior horizontal wall
void Board::addHorizontalWall(double r, int c) {
  // verify that the requested wall is valid
  // the row coordinate must be a half unit
  assert (fabs((r - floor(r))-0.5) < 0.005);
  assert (r >= 0 && r <= rows);
  assert (c >= 1 && c <= cols);
  // verify that the wall does not already exist
  assert (horizontal_walls[floor(r)][c-1] == false);
  // subtract one and round down because the corner is (0,0) not (1,1)
  horizontal_walls[floor(r)][c-1] = true;
}

// Add an interior vertical wall
void Board::addVerticalWall(int r, double c) {
  // verify that the requested wall is valid
  // the column coordinate must be a half unit
  assert (fabs((c - floor(c))-0.5) < 0.005);
  assert (r >= 1 && r <= rows);
  assert (c >= 0 && c <= cols);
  // verify that the wall does not already exist
  assert (vertical_walls[r-1][floor(c)] == false);
  // subtract one and round down because the corner is (0,0) not (1,1)
  vertical_walls[r-1][floor(c)] = true;
}


// ===================
// PRIVATE HELPER FUNCTIONS related to board geometry
// ===================

char Board::getspot(const Position &p) const {
  // verify that the requested coordinate is valid
  assert (p.row >= 1 && p.row <= rows);
  assert (p.col >= 1 && p.col <= cols);
  // subtract one from each coordinate because the corner is (0,0) not (1,1)
  return board[p.row-1][p.col-1];
}

void Board::setspot(const Position &p, char a) {
  // verify that the requested coordinate is valid
  assert (p.row >= 1 && p.row <=  rows);
  assert (p.col >= 1 && p.col <= cols);
  // subtract one from each coordinate because the corner is (0,0) not (1,1)
  board[p.row-1][p.col-1] = a;
}



// ===================
// ACCESSORS related to robots
// ===================

// Given a robot name, find the id of that robot
int Board::whichRobot(char a) const {
  for (unsigned int i = 0; i < robots.size(); i++) {
    if (robots[i] == a) {
      return i;
    }
  }
  std::cerr << "ERROR: Robot " << a << " does not exist" << std::endl;
  return 0;
}


// ===================
// MODIFIERS related to robots
// ===================

// for initial placement of a new robot
void Board::placeRobot(const Position &p, char a) {

  // check that input data is reasonable
  assert (p.row >= 1 && p.row <= rows);
  assert (p.col >= 1 && p.col <= cols);
  // make sure we don't already have a robot at that location
  assert (getspot(p) == ' ');
  // we may not start with a robot at the goal location
  assert (p != goal);

  // robots must be represented by a capital letter
  assert (a >= 'A' && a <= 'Z');

  // make sure we don't already have a robot with the same name
  for (unsigned int i = 0; i < robots.size(); i++) {
    assert (robots[i] != a);
  }

  // add the robot and its position to the vectors of robots
  robots.push_back(a);
  robot_positions.push_back(p);

  //add the initial robot position to its history
  std::vector<Position> prevTemp;
  prevTemp.push_back(p);
  prevRobotPositions.push_back(prevTemp);

  // mark the robot on the board
  setspot(p,a);
}

void getNewPos(Position &pos, const std::string &dir){
    //called by moveRobot to change a robots position in the direction its going
    if(dir == "north" ){
        pos.row = pos.row - 1;
    }
    else if(dir == "south" ){
        pos.row = pos.row + 1;
    }
    else if(dir == "east" ){
        pos.col = pos.col + 1;
    }
    else if(dir == "west" ){
        pos.col = pos.col - 1;
    }
}

bool Board::robotTherep(const char& c){
    for(int x = 0; x < robots.size(); x++){
        if(c == robots[x])
            return true;
    }
    return false;
}

bool Board::canMoveTo(const Position& pos, const std::string &dir){
    //called by moveRobots to see if a position can be moved to or not
    int xPos = pos.row - 1;
    int yPos = pos.col - 1;
    if(dir == "north" && xPos >= 0){
        if(xPos > 0){
            if(horizontal_walls[xPos][yPos] == false && robotTherep(board[xPos - 1][yPos]) == false)
                return true;
            else return false;
        }
        else if(horizontal_walls[xPos][yPos] == false)
            return true;
        else return false;
    }
    else if(dir == "south" && xPos + 1 < horizontal_walls.size()){
        if(horizontal_walls[xPos + 1][yPos] == false && robotTherep(board[xPos + 1][yPos]) == false)
            return true;
        else return false;
    }
    else if(dir == "east" && yPos + 1 < vertical_walls[xPos].size()){
        if(vertical_walls[xPos][yPos + 1] == false && robotTherep(board[xPos][yPos + 1]) == false)
            return true;
        else return false;
    }
    else if(dir == "west" && yPos >= 0){
        if(yPos > 0){
            if(vertical_walls[xPos][yPos] == false && robotTherep(board[xPos][yPos - 1]) == false)
                return true;
            else return false;
        }
        else if(vertical_walls[xPos][yPos] == false)
            return true;
        else return false;
    }
    else
        return false;
}

bool Board::moveRobot(const int& i, const std::string &direction) {
    Position pos = getRobotPosition(i);
    //if position is valid, move robot and recurse
    if(canMoveTo(pos, direction)){
        setspot(pos, ' ');
        getNewPos(pos, direction);
        setspot(pos, getRobot(i));
        robot_positions[i] = pos;
        moveRobot(i, direction);
        return true;
    }
    //else return that it could not move at all in that direction
    else
        return false;
}

bool Board::goalAchievedP(){
    //check if the goal has been achieved, handles cases where the goal robot is either defined or not
    char r = getRobot(goal_robot);
    char c = getspot(goal);
    if(r == c && goal_robot != -1)
        return true;
    else if(goal_robot == -1 && (c != ' ' && c != '?'))
        return true;
    else return false;
}


// ===================
// MODIFIER related to the puzzle goal
// ===================

void Board::setGoal(const std::string &gr, const Position &p) {
  // check that input data is reasonable
  assert (p.row >= 1 && p.row <= rows);
  assert (p.col >= 1 && p.col <= cols);

  // we may not start with a robot at the goal location
  for (unsigned int i = 0; i < robots.size(); i++) {
    assert (p != getRobotPosition(i));
  }

  // set the goal position
  goal = p;
  // set which robot must reach the goal
  if (gr == "any") {
    goal_robot = -1;
  } else {
    // otherwise it must be a single character, the name of a valid robot
    assert (gr.size() == 1);
    goal_robot = whichRobot(gr[0]);
  }
}


// ==================================================================
// PRINT THE BOARD
// ==================================================================

void Board::print() {

  // print the column headings
  std::cout << " ";
  for (int j = 1; j <= cols; j++) {
    std::cout << std::setw(4) << j;
  }
  std::cout << "\n";

  // for each row
  for (int i = 0; i <= rows; i++) {

    // don't print row 0 (it doesnt exist, the first real row is row 1)
    if (i > 0) {

      // note that each grid rows is printed as 3 rows of text, plus the separator
      // the first and third rows are blank except for vertical walls
      // the middle row has the row heading, the robot positions, and the goal
      std::string first = "  ";
      std::string middle;
      for (int j = 0; j <= cols; j++) {
        if (j > 0) {
          first += "   ";
          // determine if a robot is current located in this cell, or
          // if this is the goal
          Position p(i,j);
          char c = getspot(p);
          if (c == ' ' && p == getGoal()) {
            // if this is a goal that any robot can reach:
            if (goal_robot == -1) c = '?';
            // if this goal must be reached by a particular robot use
            // the lowercase version of the robot name to indicate
            // which robot needs to reach the goal:
            else c = tolower(getRobot(goal_robot));
          }
          // put a little space around the information so it's easier to read
          middle += " ";
          middle += c;
          middle += " ";
        }
        // the vertical walls
        if (getVerticalWall(i,j+0.5)) {
          first += "|";
          middle += "|";
        } else {
          first += " ";
          middle += " ";
        }
      }

      // output the three rows
      std::cout << first << std::endl;
      std::cout << std::setw(2) << i << middle << std::endl;
      std::cout << first << std::endl;
    }

    // print the horizontal walls between rows
    std::cout << "  +";
    for (double j = 1; j <= cols; j++) {
      (getHorizontalWall(i+0.5,j)) ? std::cout << "---" : std::cout << "   ";
      std::cout << "+";
    }
    std::cout << "\n";
  }
}

// ==================================================================
// ==================================================================
