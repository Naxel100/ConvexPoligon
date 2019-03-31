#ifndef Point_hh
#define Point_hh

#include <vector>
using namespace std;

const double epsilon = 1e-12;

/* The Point class stores a two dimensional point in the plane
   and provides some usefull operations for it. */

class Point {

public:

    //Constructor.
    Point(double x_coord = 0, double y_coord = 0);

    // Adds the coordinates of p to this point and returns this point.
    Point& operator+= (const Point& p);

    // Returns the point resulting of adding the coordinates of this point and p.
    Point operator+ (const Point& p) const;

    // Gets the x coordinate of this point.
    double get_x() const;

    // Gets the y coordinate of this point.
    double get_y() const;

    // Returns the distance to point p from this point.
    double distance(const Point& p) const;

    // Returns the slope of the line between two points.
    double slope(const Point& p) const;

    // Returns whether a point is at the left of the segment (p1, q2).
    bool leftof(const Point& p2, const Point& p3) const;

    // Returns whether a point is at the right of the segment (p1, q2).
    bool rightof(const Point& p2, const Point& p3) const;

    // Compares this point to point p.
    bool operator== (const Point& p) const;

private:

    // Coordinates of the point.
    double x, y;
};

#endif
