#include <iostream>
#include <vector>

#include "polygons.h"

int main(){
    std::vector<Point> points;
    points.push_back(Point(1,0));
    points.push_back(Point(3,0));
    points.push_back(Point(4,2));
    points.push_back(Point(0,2));
    Polygon *answer = NULL;
    std::string name = "yellow";
  try{
    answer = new EquilateralTriangle(name,points);
    std::cout << "found that shape is a EquilateralTriangle" <<std::endl;
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
	      std::cout << "found that shape is a square" <<std::endl;
	    }
	    catch (int) {
	      try {
		answer= new Rectangle(name,points);
		std::cout << "found that shape is a rectangle" <<std::endl;
	      }
	      catch (int) {
		try {
		  answer= new Rhombus(name,points);
		  std::cout << "found that shape is a rhombus" <<std::endl;
		}
		catch (int) {
		  try {
		    answer= new IsoscelesTrapezoid(name,points);
		    std::cout << "found that shape is a isoscelese trapizoid" <<std::endl;
		  }
		  catch (int) {
		    try {
		      answer= new Parallelogram(name,points);
		      std::cout << "found that shape is a parallelogram" <<std::endl;
		    }
		    catch (int) {
		      try {

			answer= new Kite(name,points);
			std::cout << "found that shape is a kite" <<std::endl;
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
    //std::cout << testShape.getName() << " has a right angle?: " << testShape.rightAnglep() <<std::endl;
    return 0;
}
