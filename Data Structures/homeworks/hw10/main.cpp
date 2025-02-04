#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "polygons.h"
#include <cstdlib>

// helper function prototypes
Polygon* CreatePolygon(const std::string &name, const std::vector<Point> &points);
void OutputStats(const std::vector<Polygon*> &polygons, std::ofstream &ostr);

// ------------------------------------------------------------------------------

int main(int argc, char* argv[]) {

  // command line arguments & opening files
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " input.txt output.txt" << std::endl;
    exit(1);
  }
  std::ifstream istr(argv[1]);
  if (!istr) {
    std::cerr << "ERROR: could not open " << argv[1] << std::endl;
    exit(1);
  }
  std::ofstream ostr(argv[2]);
  if (!ostr) {
    std::cerr << "ERROR: could not open " << argv[2] << std::endl;
    exit(1);
  }

  // the master list of polygons
  std::vector<Polygon*> polygons;

  // read the input file one line at a time
  std::string line;
  while (getline(istr,line)) {
    std::stringstream ss(line);
    std::string name, token;
    ss >> name;
    std::vector<Point> points;
    while (ss >> token) {
      std::stringstream ss2(token);
      char c;
      double x,y;
      ss2 >> c;
      assert (c == '(');
      ss2 >> x;
      ss2 >> c;
      assert (c == ',');
      ss2 >> y;
      ss2 >> c;
      assert (c == ')');
      points.push_back(Point(x,y));
    }
    assert (points.size() >= 3);
    Polygon* p = CreatePolygon(name,points);
    // add the new polygon to the master list
    polygons.push_back(p);
  }

  // write out the statistics
  OutputStats(polygons,ostr);
}

// ------------------------------------------------------------------------------

// This function determines the most specific type of shape that can
// be created from the list of points.  It does this by process of
// elimination.  Note that the order in which it attempts to create
// the shapes is important.

Polygon* CreatePolygon(const std::string &name, const std::vector<Point> &points) {
  Polygon *answer = NULL;
  try{
    answer = new EquilateralTriangle(name,points);
  }
  catch (int) {
    try {
      answer= new IsoscelesRightTriangle(name,points);
    }
    catch (int) {
      try {
	answer= new RightTriangle(name,points);
      }
      catch (int) {
	try {
	  answer= new IsoscelesTriangle(name,points);
	}
	catch (int) {
	  try {
	    answer= new Triangle(name,points);
	  }
	  catch (int) {
	    try {
	      answer= new Square(name,points);
	    }
	    catch (int) {
	      try {
		answer= new Rectangle(name,points);
	      }
	      catch (int) {
		try {
		  answer= new Rhombus(name,points);
		}
		catch (int) {
		  try {
		    answer= new IsoscelesTrapezoid(name,points);
		  }
		  catch (int) {
		    try {
		      answer= new Parallelogram(name,points);
		    }
		    catch (int) {
		      try {
			answer= new Kite(name,points);
		      }
		      catch (int) {
			try {
			  answer= new Trapezoid(name,points);
			}
			catch (int) {
			  try {
			    answer= new Quadrilateral(name,points);
			  }
			  catch (int) {
			    answer= new Polygon(name,points);
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  assert (answer != NULL);
  return answer;
}

// ------------------------------------------------------------------------------

// This function prints the output.  C++ macros are used to abbreviate
// some repetitive code.  The function call-like macros are actually
// replaced using substitution by the preprocessor before the code is
// given to the compiler.  (You are not required to understand the
// details of the macros.  You do not need to edit this code.)

void OutputStats(const std::vector<Polygon*> &polygons, std::ofstream &ostr) {

  // define and initialize variables
# define InitializeCount(type) std::vector<std::string> all_##type
  InitializeCount(Polygon);
  InitializeCount(Triangle);
  InitializeCount(IsoscelesTriangle);
  InitializeCount(RightTriangle);
  InitializeCount(IsoscelesRightTriangle);
  InitializeCount(EquilateralTriangle);
  InitializeCount(Quadrilateral);
  InitializeCount(Trapezoid);
  InitializeCount(Kite);
  InitializeCount(Parallelogram);
  InitializeCount(IsoscelesTrapezoid);
  InitializeCount(Rhombus);
  InitializeCount(Rectangle);
  InitializeCount(Square);
  std::vector<std::string> equal_sides;
  std::vector<std::string> equal_angles;
  std::vector<std::string> right_angle;


  // count & record the names of shapes in each category
  for (std::vector<Polygon*>::const_iterator i = polygons.begin(); i!=polygons.end(); ++i) {
#   define IncrementCount(type) if (dynamic_cast<type*> (*i)) all_##type.push_back((*i)->getName())
    IncrementCount(Polygon);
    IncrementCount(Triangle);
    IncrementCount(IsoscelesTriangle);
    IncrementCount(RightTriangle);
    IncrementCount(IsoscelesRightTriangle);
    IncrementCount(EquilateralTriangle);
    IncrementCount(Quadrilateral);
    IncrementCount(Trapezoid);
    IncrementCount(Kite);
    IncrementCount(Parallelogram);
    IncrementCount(IsoscelesTrapezoid);
    IncrementCount(Rhombus);
    IncrementCount(Rectangle);
    IncrementCount(Square);
    if ((*i)->HasAllEqualSides()) equal_sides.push_back((*i)->getName());
    if ((*i)->HasAllEqualAngles()) equal_angles.push_back((*i)->getName());
    if ((*i)->HasARightAngle()) right_angle.push_back((*i)->getName());
  }

  // output data for each category
# define PrintCount(type) ostr << all_##type.size() << " " #type"(s): "; \
  for (unsigned int j = 0; j < all_##type.size(); j++) { ostr << " " << all_##type[j]; } ostr << std::endl
  PrintCount(Polygon);
  PrintCount(Triangle);
  PrintCount(IsoscelesTriangle);
  PrintCount(RightTriangle);
  PrintCount(IsoscelesRightTriangle);
  PrintCount(EquilateralTriangle);
  PrintCount(Quadrilateral);
  PrintCount(Trapezoid);
  PrintCount(Kite);
  PrintCount(Parallelogram);
  PrintCount(IsoscelesTrapezoid);
  PrintCount(Rhombus);
  PrintCount(Rectangle);
  PrintCount(Square);
  ostr << equal_sides.size() << " Shape(s) with all equal sides: ";
  for (unsigned int i = 0; i < equal_sides.size(); i++) {
    ostr << " " << equal_sides[i];
  }
  ostr << std::endl;
  ostr << equal_angles.size() << " Shape(s) with all equal angles: ";
  for (unsigned int i = 0; i < equal_angles.size(); i++) {
    ostr << " " << equal_angles[i];
  }
  ostr << std::endl;
  ostr << right_angle.size() << " Shape(s) with a right angle: ";
  for (unsigned int i = 0; i < right_angle.size(); i++) {
    ostr << " " << right_angle[i];
  }
  ostr << std::endl;
}

// ------------------------------------------------------------------------------
