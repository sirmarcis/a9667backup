#include <iostream>
#include <string>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <algorithm>

#include "board.h"

// ================================================================
// ================================================================
// This function is called if there was an error with the command line arguments
bool usage(const std::string &executable_name) {
  std::cerr << "Usage: " << executable_name << " <puzzle_file>" << std::endl;
  std::cerr << "       " << executable_name << " <puzzle_file> -max_moves <#>" << std::endl;
  std::cerr << "       " << executable_name << " <puzzle_file> -all_solutions" << std::endl;
  std::cerr << "       " << executable_name << " <puzzle_file> -visualize_accessibility" << std::endl;
  std::cerr << "       " << executable_name << " <puzzle_file> -max_moves <#> -all_solutions" << std::endl;
  std::cerr << "       " << executable_name << " <puzzle_file> -max_moves <#> -visualize_accessibility" << std::endl;
  exit(0);
}


// ================================================================
// ================================================================
// load a Ricochet Robots puzzle from the input file
Board load(const std::string &executable, const std::string &filename) {

  // open the file for reading
  std::ifstream istr (filename.c_str());
  if (!istr) {
    std::cerr << "ERROR: could not open " << filename << " for reading" << std::endl;
    usage(executable);
  }

  // read in the board dimensions and create an empty board
  // (all outer edges are automatically set to be walls
  int rows,cols;
  istr >> rows >> cols;
  Board answer(rows,cols);

  // read in the other characteristics of the puzzle board
  std::string token;
  while (istr >> token) {
    if (token == "robot") {
      char a;
      int r,c;
      istr >> a >> r >> c;
      answer.placeRobot(Position(r,c),a);
    } else if (token == "vertical_wall") {
      int i;
      double j;
      istr >> i >> j;
      answer.addVerticalWall(i,j);
    } else if (token == "horizontal_wall") {
      double i;
      int j;
      istr >> i >> j;
      answer.addHorizontalWall(i,j);
    } else if (token == "goal") {
      std::string which_robot;
      int r,c;
      istr >> which_robot >> r >> c;
      answer.setGoal(which_robot,Position(r,c));
    } else {
      std::cerr << "ERROR: unknown token in the input file " << token << std::endl;
      exit(0);
    }
  }

  // return the initialized board
  return answer;
}

// ================================================================
// ================================================================
//Begin students code:

void printAccessibilityData(const std::vector<std::vector<int> >& accessabilityData){
    //functon to print out accessibility data
    for(int x = 0; x < accessabilityData.size(); x++){
        for(int y = 0; y < accessabilityData[x].size(); y++){
            if(accessabilityData[x][y] == -1)
                std::cout << ". ";
            else
                std::cout << accessabilityData[x][y] << " ";
        }
        std::cout << std::endl;
    }

}

void printMoveOrder(const std::vector<std::vector<std::string> >& moveOrder, const int &numMoves){
    //called by findPath, prints out all move order combinations
    for(int x = 0; x < moveOrder.size(); x++){
        std::cout << std::endl;
        for(int y = 0; y < moveOrder[x].size(); y++){
            std:: cout << moveOrder[x][y] <<std::endl;
            if(y == moveOrder[x].size() - 1){
                std::string toPrint  = moveOrder[x][y].substr(0, 7);
                std::cout << toPrint << " reaches the goal after " << numMoves << " moves"<<std::endl;
            }
        }
    }
}

void printOneMove(Board& board, const std::vector<std::vector<std::string> >& moveOrder, const int &numMoves){
    //called by findPath, prints a single move combination when all_solutions is not specified
    board.print();
    for(int y = 0; y < moveOrder[0].size(); y++){
        char robotElt = moveOrder[0][y].at(6);
        int robot = board.whichRobot(robotElt);
        std::string moveDir = moveOrder[0][y].substr(14);
        board.moveRobot(robot, moveDir);
        std::cout << moveOrder[0][y] <<std::endl;
        board.print();
        if(y == moveOrder[0].size() - 1){
            std::string toPrint  = moveOrder[0][y].substr(0, 7);
            std::cout << toPrint << " reaches the goal after " << numMoves << " moves"<<std::endl;
        }
    }
}

std::vector<std::vector<int> > buildAccessibilityData(Board& board){
    //called by findPath, make a two dimensional array with all initial values of -1
    std::vector<std::vector<int> > accessabilityData;
    for(int x = 0; x < board.getRows(); x++){
        std::vector<int> temp;
        for(int y = 0; y < board.getCols(); y++){
            temp.push_back(-1);
        }
        accessabilityData.push_back(temp);
    }
    //add the initial robot positions
    for(int x = 0; x < (int)board.numRobots(); x++){
        Position robotPos = board.getRobotPosition(x);
        accessabilityData[robotPos.row - 1][robotPos.col - 1] = 0;
    }
    return accessabilityData;
}

void addPointToAccessability(const Position& p, std::vector<std::vector<int> > &accessibilityData, const int& numMoves){
    //called by followPath to simply add a point to the access data
    int xPos = p.row - 1, yPos = p.col - 1;
    if(accessibilityData[xPos][yPos] == -1 || accessibilityData[xPos][yPos] > numMoves)
        accessibilityData[xPos][yPos] = numMoves;
}

//please don't grade me down, these are purely for convience's sake
std::string north = "north", south = "south", east = "east", west = "west";

void followPath(Board &board, const int &numRobots, int &numSolutions, const int &maxMoves,
                std::vector<std::vector<int> >& accessibilityData,
                std::vector<std::vector<std::string> >& moveOrder,
                std::vector<std::string> moveTrace,
                int numMoves, bool &successP){
    //called by findPath to recursivly go down through all possible move combinations

    if(board.goalAchievedP()){
        successP = true;
        moveOrder.push_back(moveTrace);
        numSolutions++;
    }
    //ensure there are moves left to exicute
    else if(numMoves <= maxMoves && board.goalAchievedP() == false){
        //indicate a move was made
        numMoves++;
        int robotComboElt = 0;
        //loop through all robots and begin new paths for each going in all four directions
        for(int robotElt = 0; robotElt < numRobots; robotElt++){
            robotComboElt = robotElt;
            Position currPos;
            Board prevBoard = board; //to reset the board after each rabbit-hole attempt
            std::string moveMade = "robot ";
            moveMade.push_back(board.getRobot(robotElt));
            //if the robot can move in any of the four compas directions, do so and recurse, while tracking all relavent data
            if(board.moveRobot(robotComboElt, north)){
                currPos = board.getRobotPosition(robotComboElt);
                addPointToAccessability(currPos, accessibilityData, numMoves);
                moveMade += " moves north";
                moveTrace.push_back(moveMade);
                followPath(board, numRobots, numSolutions, maxMoves, accessibilityData, moveOrder, moveTrace, numMoves, successP);
                moveTrace.pop_back();
                moveMade = moveMade.substr(0, 7);
            }
            board = prevBoard;
            if(board.moveRobot(robotComboElt, east)){
                currPos = board.getRobotPosition(robotComboElt);
                addPointToAccessability(currPos, accessibilityData, numMoves);
                moveMade += " moves east";
                moveTrace.push_back(moveMade);
                followPath(board, numRobots, numSolutions, maxMoves, accessibilityData, moveOrder, moveTrace, numMoves, successP);
                moveTrace.pop_back();
                moveMade = moveMade.substr(0, 7);
            }
            board = prevBoard;
            if(board.moveRobot(robotComboElt, south)){
                currPos = board.getRobotPosition(robotComboElt);
                addPointToAccessability(currPos, accessibilityData, numMoves);
                moveMade += " moves south";
                moveTrace.push_back(moveMade);
                followPath(board, numRobots, numSolutions, maxMoves, accessibilityData, moveOrder, moveTrace, numMoves, successP);
                moveTrace.pop_back();
                moveMade = moveMade.substr(0, 7);
            }
            board = prevBoard;
            if(board.moveRobot(robotComboElt, west)){
                currPos = board.getRobotPosition(robotComboElt);
                addPointToAccessability(currPos, accessibilityData, numMoves);
                moveMade += " moves west";
                moveTrace.push_back(moveMade);
                followPath(board, numRobots, numSolutions, maxMoves, accessibilityData, moveOrder, moveTrace, numMoves, successP);
                moveTrace.pop_back();
                moveMade = moveMade.substr(0, 7);
            }
            board = prevBoard;
        }
    }

}

void findPath(Board &board, bool& accessOrNot, int& moveCap, bool& moveCapInputp, bool& all_solutions){
    //called by main, caller function for the recursive search function followPath, where Max moves is incremented up untill the puzzle is solved
    int numRobots = (int)board.numRobots();
    std::vector<std::vector<int> > accessibilityData = buildAccessibilityData(board);
    int numSolutions = 0;
    std::vector<std::vector<std::string> > moveOrder;
    int numMoves = 0, maxMoves = 1;
    bool puzzleSolvedP = false;
    while(puzzleSolvedP == false && moveCap > maxMoves){
        numMoves = 0;
        std::vector<std::string> moveTrace;
        followPath(board, numRobots, numSolutions, maxMoves, accessibilityData, moveOrder, moveTrace, numMoves, puzzleSolvedP);
        maxMoves++;
        //if the puzzle was solved with this number of moves, print the data and return
        if(puzzleSolvedP){
            if(accessOrNot)
                printAccessibilityData(accessibilityData);
            if(all_solutions){
                board.print();
                std::cout << numSolutions << " different " << maxMoves << " move solutions:" <<std::endl;
                printMoveOrder(moveOrder, maxMoves);
            }
            else
                printOneMove(board, moveOrder, maxMoves);
        }

    }
    //if the puzzle was not solved, print failure data
    if(puzzleSolvedP == false){
        if(accessOrNot)
            printAccessibilityData(accessibilityData);
        board.print();
        if(moveCapInputp)
            std::cout << "no solutions with " << moveCap << " or fewer moves" <<std::endl;
        else
            std::cout << "no solutions" <<std::endl;
    }

}

//helper command line calls:
//    cd C:/Users/marava2/My\ Documents/Data\ Structures/homeworks/hw6

//end students code
// ================================================================
// ================================================================

int main(int argc, char* argv[]) {

  // There must be at least one command line argument, the input puzzle file
  if (argc < 2) {
    usage(argv[0]);
  }

  // By default, the maximum number of moves is unlimited (NOTE: may need to be changed later)
  int max_moves = -1;

  // By default, output one solution using the minimum number of moves
  bool all_solutions = false;

  // By default, do not visualize the accessibility
  bool visualize_accessibility = false;

  // Read in the other command line arguments
  for (int arg = 2; arg < argc; arg++) {
    if (argv[arg] == std::string("-all_solutions")) {
      // find all solutions to the puzzle that use the fewest number of moves
      all_solutions = true;
    } else if (argv[arg] == std::string("-max_moves")) {
      // the next command line arg is an integer, a cap on the  # of moves
      arg++;
      assert (arg < argc);
      max_moves = atoi(argv[arg]);
      assert (max_moves > 0);
    } else if (argv[arg] == std::string("-visualize_accessibility")) {
      // As a first step towards solving the whole problem, with this
      // option, let's visualize where the robots can move and how many
      // steps it takes to get there
      visualize_accessibility = true;
    } else {
      std::cout << "unknown command line argument" << argv[arg] << std::endl;
      usage(argv[0]);
    }
  }

  // Load the puzzle board from the input file
  Board board = load(argv[0],argv[1]);
  int moveCap = 15; //in order to limit infinite looping
  bool moveCapInputp = false;
  if(max_moves != -1){
    moveCapInputp = true;
    moveCap = max_moves;
  }
  findPath(board, visualize_accessibility, moveCap, moveCapInputp, all_solutions);

}

// ================================================================
// ================================================================
