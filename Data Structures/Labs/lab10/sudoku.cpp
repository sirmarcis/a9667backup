#include <vector>
#include <set>
#include <iostream>
#include <math.h>
#include <cassert>
#include "sudoku.h"

using std::set;
using std::vector;
using std::cout;
using std::cin;
using std::endl;

// ======================================================
// A program to solve very simple Sudoku puzzles
// ======================================================

int main() {

  int size;
  while (cin >> size) {
    // Read in the board size and create an empty board
    Sudoku board(size);

    // Read in and set the known positions in the board
    char c;
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        cin >> c;
        if (c != '.') {
          board.Set(i,j,c-'0');
        }
      }
    }

    // The iterative solving routine
    while (1) {

      // Is the puzzle is already solved?
      if (board.IsSolved()) {
        cout << "\nPuzzle is solved:" << endl;
        board.Print();
        break;
      }

      // Is the puzzle impossible?
      if (board.Impossible()) {
        cout << "\nCannot solve this puzzle:" << endl;
        board.Print();
        break;
      }

      // Try to reduce the number of choices in one or more board cells
      // by propagating known values within each row, column, and quadrant
      int changed = 0;
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          if (board.KnownValue(i,j))
            changed += board.Propagate(i,j);
        }
      }
      // If nothing changed this iteration, give up
      if (changed == 0) {
        cout << "\nPuzzle is too difficult for this solver!" << endl;
        board.Print();
        break;
      }
    }
  }
}

// ======================================================

// Construct an empty board
Sudoku::Sudoku(int s) {
    // set the size
    size = s;
    quadrant_size = (int)sqrt(size);
    assert (quadrant_size*quadrant_size == s);
    std::set<int> newNode;
    for(int z = 1; z <= size; z ++)
        newNode.insert(z);
    std::vector<std::set<int> > newRow;
    for(int y = 0; y < size; y++){
        newRow.push_back(newNode);
    }
    for(int x = 0; x < size; x++){
        board.push_back(newRow);
    }

}

// To construct the puzzle, set the value of a particular cell
void Sudoku::Set(int i, int j, int value) {
  // make sure the value is one of the legal values
  assert (board[i][j].find(value) != board[i][j].end());
  board[i][j].clear();
  board[i][j].insert(value);
}

// Does the cell have exactly one legal value?
bool Sudoku::KnownValue(int i, int j) const {
  return (board[i][j].size() == 1);
}

// If there is exactly one number in each cell of the grid, the puzzle is solved!
bool Sudoku::IsSolved() const {
    bool solved = true;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j].size() != 1)
                return false;
        }
    }
    return solved;
}

// If there are no legal choices in one or more cells of the grid, the puzzle is impossible
bool Sudoku::Impossible() const {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (board[i][j].size() == 0)
        return true;
    }
  }
  return false;
}

// print out the solution
void Sudoku::Print() const {
  int solved = IsSolved();
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      int count = size+1;
      for (set<int>::iterator itr = board[i][j].begin(); itr != board[i][j].end(); itr++) {
        count--;
        cout << *itr;
      }
      if (!solved) {
        // if it's not solved, print out spaces to separate the values
        for (int k = 0; k < count; k++) {
          cout << " ";
        }
      }
    }
    cout << endl;
  }
}


// Propagate information from this cell along columns & diagonals &
// within quadrant. Return the number of cells that changed.
int Sudoku::Propagate(int i, int j) {
    if(board[i][j].size() == 1){
        int cellsChanged = 0;
        int numToRemove = *(board[i][j].begin());
        for(int row = 0; row < size; row++){// empty row of found value
            if(row != i){
                //std::cout << "old set size: " << board[row][j].size() <<std::endl;
                cellsChanged += board[row][j].erase(numToRemove);
                //std::cout << "new set size: " << board[row][j].size() <<std::endl;
            }
        }
        for(int col = 0; col < board[i].size() ;col++){ // empty column of found value
            if(col != j){
                cellsChanged += board[i][col].erase(numToRemove);
            }
        }

        int quadRowStart = 0, quadRowEnd = 0, quadColStart = 0, quadColEnd = 0;
        quadRowStart = (i / quadrant_size) * quadrant_size;
        quadRowEnd = quadRowStart + quadrant_size;
        quadColStart = (j / quadrant_size) * quadrant_size;
        quadColEnd = quadColStart + quadrant_size;
        for(int x = quadRowStart; x < quadRowEnd; x++){
            for(int y = quadColStart; y < quadColEnd; y++){
                if(x != i && y != j && board[x][y].find(numToRemove) != board[x][y].end()){
                    cellsChanged += board[x][y].erase(numToRemove);
                }
            }
        }

        return cellsChanged;
    }
    return 0;
}
