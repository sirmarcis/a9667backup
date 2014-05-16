#ifndef POLYGONS_H_INCLUDED
#define POLYGONS_H_INCLUDED

#include <string>
#include <vector>
#include "utilities.h"


//the "master" class, every other shape will inherit this one
class Polygon {
public:
    //CONSTRUCTOR
    Polygon(const std::string &nName, const std::vector<Point> &nPoints) : name(nName), points(nPoints) {}

    //ACCESSORS (Used outside of class implementations)
    virtual std::string getName() { return name; } //its virtual in order to make this class polymorphic
    const int numPoints() const { return points.size(); }
    const bool HasAllEqualSides() const;
    const bool HasAllEqualAngles() const;

    const bool HasARightAngle() const;  //this accessor is used both outside of class implementations and within them

    //PUBLIC CHILDREN ACCESSORS (used in future implementations of this class's children (it is their birthright!))
    const int numEqualSides() const; //gets the number of equal sides witin the shape
    const bool adjacentAnglep() const;
    const int numParallelLines() const; //finds the number of parallel lines within the shape

protected:
    //MEMBER VARIABLES
    std::string name;
    std::vector<Point> points;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ALL TRIANGLE CLASS DECLARATIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//the triangle shape, is a child of polygon and is still one of the most basic shapes
class Triangle : public Polygon {
public:
    Triangle(const std::string &nName, const std::vector<Point> &nPoints);
};

class IsoscelesTriangle : virtual public Triangle{
public:
    IsoscelesTriangle(const std::string &nName, const std::vector<Point> &nPoints);
};

class RightTriangle : virtual public Triangle{
public:
    RightTriangle(const std::string &nName, const std::vector<Point> &nPoints);
};

class EquilateralTriangle : public IsoscelesTriangle{
public:
    EquilateralTriangle(const std::string &nName, const std::vector<Point> &nPoints);
};

//this shape does not have an implementation because it is based on if both of its parents test cases are true
class IsoscelesRightTriangle : public RightTriangle, public IsoscelesTriangle{
public:
    IsoscelesRightTriangle(const std::string &nName, const std::vector<Point> &nPoints)
    : Triangle(nName,nPoints), RightTriangle(nName,nPoints), IsoscelesTriangle(nName,nPoints) {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//END TRIANGLE DECLARATIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ALL QUADRILATERAL CLASS DECLARATIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Quadrilateral : public Polygon {
public:
    Quadrilateral(const std::string &nName, const std::vector<Point> &nPoints);
};

class Trapezoid : virtual public Quadrilateral {
public:
    Trapezoid(const std::string &nName, const std::vector<Point> &nPoints);
};

class Kite : virtual public Quadrilateral {
public:
    Kite(const std::string &nName, const std::vector<Point> &nPoints);
};

class IsoscelesTrapezoid : virtual public Trapezoid {
public:
    IsoscelesTrapezoid(const std::string &nName, const std::vector<Point> &nPoints);
};

class Parallelogram : virtual public Trapezoid {
public:
    Parallelogram(const std::string &nName, const std::vector<Point> &nPoints);
};

class Rectangle : virtual public Parallelogram, public IsoscelesTrapezoid{
public:
    Rectangle(const std::string &nName, const std::vector<Point> &nPoints);
};

class Rhombus : virtual public Parallelogram, public Kite{
public:
    Rhombus(const std::string &nName, const std::vector<Point> &nPoints);
};

//this shape does not have an implementation because it is based on if both of its parents test cases are true
class Square : public Rhombus, public Rectangle{
public:
    Square(const std::string &nName, const std::vector<Point> &nPoints)
    : Quadrilateral(nName,nPoints), Trapezoid(nName,nPoints), Parallelogram(nName,nPoints), Rhombus(nName,nPoints), Rectangle(nName,nPoints) {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//END QUADRILATERAL DECLARATIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#endif // POLYGONS_H_INCLUDED
