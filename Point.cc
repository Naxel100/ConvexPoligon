#include "Point.hh"
#include <cmath>
using namespace std;

// Returns x squared.
static double sqr (double x) {
    return x*x;
}

// Constructor.
Point::Point(double x_coord, double y_coord) {
    x = x_coord;
    y = y_coord;
}

/** Adds the coordinates of p to this point and returns this point. */
Point& Point::operator+= (const Point& p) {
    x += p.x;
    y += p.y;
    return *this;
}

// Returns the point resulting of adding the coordinates of this point and p.
Point Point::operator+ (const Point& p) const {
    Point q = p;
    q += *this;
    return q;
    // Equivalent one-liner: return Point(p) += *this;
}

// Gets the x coordinate of this point.
double Point::get_x () const {
    return x;
}

// Gets the y coordinate of this point.
double Point::get_y () const {
    return y;
}

// Returns the distance to point p from this point.
double Point::distance (const Point& p) const {
    return sqrt(sqr(x - p.x) + sqr(y - p.y));
}

// Returns the slope of the line between two points.
double Point::slope(const Point& p) const {
    return (p.get_y() - y) / (p.get_x() - x);
}

// Returns whether a point is at the left of the segment (p1, q2).
bool Point::leftof(const Point& p1, const Point& p2) const {
    double val = (p2.get_x() - p1.get_x()) * (y - p1.get_y()) -
                 (p2.get_y() - p1.get_y()) * (x - p1.get_x());
    return val >= 0 or abs(val) < epsilon;
}

// Returns whether a point is at the right of the segment (p1, q2).
bool Point::rightof(const Point& p1, const Point& p2) const {
    double val = (p2.get_x() - p1.get_x()) * (y - p1.get_y()) -
                 (p2.get_y() - p1.get_y()) * (x - p1.get_x());
    return val <= 0 or abs(val) < epsilon;
}

// Compares this point to point p.
bool Point::operator== (const Point& p) const {
    return x == p.x and y == p.y;
}
