#include <cassert>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>


// ==================================================================
// ==================================================================
// A tiny all-public helper class to record a 2D board position

class Position {
public:
  // the coordinate (-1,-1) is invalid/unitialized
  Position(int r=-1,int c=-1) : row(r),col(c) {}
  int row,col;
};

// convenient functions to print and equality compare Positions
std::ostream& operator<<(std::ostream &ostr, const Position &p);
bool operator==(const Position &a, const Position &b);
bool operator!=(const Position &a, const Position &b);


// ==================================================================
// ==================================================================
// A class to hold information about the puzzle board including the
// dimensions, the location of all walls, the current position of all
// robots, the goal location, and the robot (if specified) that must
// reach that position

class Board {
public:

  // CONSTRUCTOR
  Board(int num_rows, int num_cols);


  // ACCESSORS related the board geometry
  int getRows() const { return rows; }
  int getCols() const { return cols; }
  bool getHorizontalWall(double r, int c) const;
  bool getVerticalWall(int r, double c) const;

  // ACCESSORS related to the robots and their current positions
  unsigned int numRobots() const { return robots.size(); }
  // lookup the assigned "id" for a robot by name
  int whichRobot(char a) const;
  // given a robot's id, lookup the name
  char getRobot(int i) const { return robots[i]; }
  // get the robots current position
  Position getRobotPosition(int i) const { return robot_positions[i]; }

  // ACCESSORS related to the overal puzzle goal target location
  // the position
  Position getGoal() const { return goal; }
  // the id of the robot that must reach the goal position
  // (if any robot is allowed to reach the goal, this value is -1)
  int getGoalRobot() const { return goal_robot; }


  // MODIFIERS related to board geometry
  void addHorizontalWall(double r, int c);
  void addVerticalWall(int r, double c);

  // MODIFIERS related robot position
  // initial placement of a new robot
  void placeRobot(const Position &p, char a);
  // move an existing robot
  bool canMoveTo(const Position& pos, const std::string &dir);
  bool moveRobot(const int& i, const std::string &direction);
  bool goalAchievedP();

  // MODIFIER related to the puzzle goal
  void setGoal(const std::string &goal_robot, const Position &p);

  // PRINT
  void print();


private:

  // private helper functions
  char getspot(const Position &p) const;
  void setspot(const Position &p, char a);
  bool robotTherep(const char& c);


  // REPRESENTATION

  // the board geometry
  int rows;
  int cols;
  std::vector<std::vector<char> > board;
  std::vector<std::vector<bool> > vertical_walls;
  std::vector<std::vector<bool> > horizontal_walls;

  // information about the names and current positions of the robots
  std::vector<char> robots;
  std::vector<Position> robot_positions;
  std::vector<std::vector<Position> > prevRobotPositions;

  // the goal position and the id of the robot that must reach it
  Position goal;
  // the goal robot is -1 if the puzzle is solved if any robot reaches the goal
  int goal_robot;
};
