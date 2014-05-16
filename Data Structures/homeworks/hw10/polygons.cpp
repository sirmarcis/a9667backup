#include <iostream>
#include <string>
#include <vector>

#include "polygons.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//POLYGON CLASS IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const bool Polygon::HasAllEqualSides() const{
    //see implementation of numEqualSides for original version of this code (does it the same way)
    std::vector<double> sideLengths; //store the lengths of all the sides
    //first, handle the corner case...
    sideLengths.push_back(DistanceBetween(points[points.size()-1], points[0]));
    for(int x = 0; x < points.size()-1; x++){ //then, find all standard lengths
        double currSideLength = DistanceBetween(points[x], points[x+1]);
        for(int y = 0; y < sideLengths.size(); y++){ //and compare the new length with all previous ones
            if(!EqualSides(currSideLength, sideLengths[y])) //if the side is not equal to any other, return false
                return false;
        }
        sideLengths.push_back(currSideLength); //before adding it into the array
    }
    return true;
}

const bool Polygon::HasAllEqualAngles() const{
    //same basic implementation as numEqualSides, except for one function call
    std::vector<double> angles; //store all the angle sizes
    //first, handle the corner case...
    angles.push_back(Angle(points[points.size()-1], points[0], points[1]));
    for(int x = 1; x < points.size()-1; x++){ //then, find all standard angles
        double currAngle = Angle(points[x-1],points[x], points[x+1]);
        for(int y = 0; y < angles.size(); y++){ //and compare the new angle with all previous ones
            if(!EqualAngles(currAngle, angles[y])) //if the angle is not equal to any other, return false
                return false;
        }
        angles.push_back(currAngle); //before adding it into the array
    }
    //then, handle the back corner case (the angle made by the second to last point, last point, and the first point)
    if(!EqualAngles(angles[angles.size()-1], Angle(points[points.size()-2], points[points.size()-1], points[0])))
        return false;
    return true;
}

//part of the Polygon class, finds if there are any right angles in the shape
const bool Polygon::HasARightAngle() const{
    for(int x = 1; x < points.size()-1; x++){ //check each middle point to see if its at a right angle
        double currAngle = Angle(points[x-1], points[x], points[x+1]);
        if(RightAngle(currAngle))
            return true;
    }
    //then, handle edge cases at the begining and end of the points array
    Point firstPoint = points[0];
    Point lastPoint = points[points.size()-1];
    Point secondToLast = points[points.size()-2];
    Point afterFirst = points[1];
    double firstAngle = Angle(lastPoint, firstPoint, afterFirst); //loop from the end back around to the front of the array
    double lastAngle = Angle(secondToLast, lastPoint, firstPoint);
    if(RightAngle(firstAngle) || RightAngle(lastAngle))
        return true;
    return false;
}

const int Polygon::numEqualSides() const{
    int equalSides = 0; //starts out with no side pairs being equal
    std::vector<double> sideLengths; //store the lengths of all the sides
    //first, handle the corner case...
    sideLengths.push_back(DistanceBetween(points[points.size()-1], points[0]));
    for(int x = 0; x < points.size()-1; x++){ //then, find all standard lengths
        double currSideLength = DistanceBetween(points[x], points[x+1]);
        for(int y = 0; y < sideLengths.size(); y++){ //and compare the new length with all previous ones
            if(EqualSides(currSideLength, sideLengths[y])) //adding to the count when any are found to be equal
                equalSides++;
        }
        sideLengths.push_back(currSideLength); //before adding it into the array
    }
    return equalSides;
}

const bool Polygon::adjacentAnglep() const{
    //same basic implementation as numEqualSides, except for a couple function calls (we only need to check three of the four angles)
    std::vector<double> angles; //store all the angle sizes
    //first, handle the corner case...
    angles.push_back(Angle(points[points.size()-1], points[0], points[1]));
    for(int x = 1; x < points.size()-1; x++){ //then, find all standard angles
        double currAngle = Angle(points[x-1], points[x], points[x+1]);
        if(EqualAngles(currAngle, angles[angles.size()-1])) //if the angle is equal to one next to it, return true
            return true;
        angles.push_back(currAngle); //before adding it into the array
    }
    return false;
}

const int Polygon::numParallelLines() const{
    int parallelSides = 0; //starts out with no sides being parallel
    std::vector<Vector> sides; //stores the sides for comparison
    //as usual, handle the corner case to get us going...
    sides.push_back(Vector(points[points.size()-1], points[0]));
    for(int x = 0; x < points.size()-1; x++){ //then, add all the standard lengths to the array
        Vector currVec(points[x],points[x+1]);
        for(int y = 0; y < sides.size(); y++){
            if(Parallel(currVec, sides[y]))
                parallelSides++;
        }
        sides.push_back(currVec);
    }
    return parallelSides;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//END POLYGON CLASS IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////NOTE: the following shape implementations are split up into two categories; triangles and quadrilaterals.  The order of implementation
/////////follows the diagram as it decends from polygon and goes from right to left



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ALL TRIANGLE IMPLEMENTATIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//the constructor for the triangle shape, the check is that it must have three points
Triangle::Triangle(const std::string &nName, const std::vector<Point> &nPoints) : Polygon(nName,nPoints){
    if(numPoints() != 3) //if there are not three points, the shape is not a triangle and so throw an exception
        throw 1;
}

IsoscelesTriangle::IsoscelesTriangle(const std::string &nName, const std::vector<Point> &nPoints) : Triangle(nName,nPoints){
    if(numEqualSides() < 1) //if there is less than one pair of sides equal, throw up
        throw 1;
}

RightTriangle::RightTriangle(const std::string &nName, const std::vector<Point> &nPoints) : Triangle(nName,nPoints){
    if(!HasARightAngle()) //if it does not have a right angle, barf
        throw 1;
}

EquilateralTriangle::EquilateralTriangle(const std::string &nName, const std::vector<Point> &nPoints) : Triangle(nName,nPoints), IsoscelesTriangle(nName,nPoints){
    if(numEqualSides() < 2) //if all three sides are not equal, spew
        throw 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//END TRIANGLE IMPLEMENTATIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ALL QUADRILATERAL IMPLEMENTATIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Quadrilateral::Quadrilateral(const std::string &nName, const std::vector<Point> &nPoints) : Polygon(nName,nPoints){
    if(numPoints() != 4) //if there are not four points, the shape is not a quadrilateral and so throw an exception
        throw 1;
}

Trapezoid::Trapezoid(const std::string &nName, const std::vector<Point> &nPoints) : Quadrilateral(nName,nPoints){
    if(numParallelLines() < 1) //if the shape does not have at least one pair of parallel sides, upchuck
        throw 1;
}

Kite::Kite(const std::string &nName, const std::vector<Point> &nPoints) : Quadrilateral(nName,nPoints){
    if(numEqualSides() < 1) //if the shape does not have at least one pair of equal sides, excrete through the mouth
        throw 1;
}

IsoscelesTrapezoid::IsoscelesTrapezoid(const std::string &nName, const std::vector<Point> &nPoints): Quadrilateral(nName,nPoints), Trapezoid(nName,nPoints){
    if((numEqualSides() < 1) || !adjacentAnglep()) //an Isosceles Trapezoid needs to have a pair of equal sides and a pair of equal angles
        throw 1;
}

Parallelogram::Parallelogram(const std::string &nName, const std::vector<Point> &nPoints) : Quadrilateral(nName,nPoints), Trapezoid(nName,nPoints){
    if(numParallelLines() < 2) //must have two pairs of parallel sides
        throw 1;
}

Rectangle::Rectangle(const std::string &nName, const std::vector<Point> &nPoints) : Quadrilateral(nName,nPoints), Trapezoid(nName,nPoints), Parallelogram(nName,nPoints), IsoscelesTrapezoid(nName,nPoints){
    if((numEqualSides() < 2) || !HasARightAngle()) // must have more than one pair of equal sides and a right angle
        throw 1;
}

Rhombus::Rhombus(const std::string &nName, const std::vector<Point> &nPoints) : Quadrilateral(nName,nPoints), Trapezoid(nName,nPoints), Parallelogram(nName,nPoints), Kite(nName,nPoints) {
    if(numEqualSides() < 6) // numEqual sides gives the factorial of the number of equal sides, which is unimportant as long as this fact is taken into account
        throw 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//END QUADRILATERAL IMPLEMENTATIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


