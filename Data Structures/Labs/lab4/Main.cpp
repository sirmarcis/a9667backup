#include <iostream>
#include "Rectangle.h"

int
main(){
    Point2D p1(3, 3);
    Point2D p2(6, 6);
    Rectangle r1(p1, p2);
    Point2D p3(2, 2);
    std::cout << "point p3, which is (" << p3.x() << ", " << p3.y() << ") has been added: [" << r1.is_point_within(p3) << "] " << std::endl;
    r1.add_point(p3);
    print_rectangle(r1);
    Rectangle r2(p3, p1);
    r1.add_point(p3);
    //std::cout << "triangles r1 and r2 share [" << r1.points_contained().size() << "] points" <<std::endl;
    return 0;
}
