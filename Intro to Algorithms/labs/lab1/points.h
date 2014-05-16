#ifndef POINTS_H_INCLUDED
#define POINTS_H_INCLUDED

#include <iostream>
#include <math.h>

class Point{
public:
	Point(const double x1, const double y1) : x(x1), y(y1) {}


	double getX() {return x; }
	double getY() {return y; }

	double dist(Point b) {return sqrt(pow((b.getX() - x), 2) + pow((b.getY() - y), 2)); }

private:
	double x;
	double y;
};

#endif // POINTS_H_INCLUDED
