#include "Point.hh"

#include <cmath>
using namespace std;



/* Returns the square of a number. It is marked static to be private in this module. */
static double sqr (double x) {
    return x*x;
}


/** Constructor. */
Point::Point(double x_coord, double y_coord) {
    x = x_coord;
    y = y_coord;
}



/** Gets the x coordinate of this point. */
double Point::get_x () const {
    return x;
}


/** Gets the y coordinate of this point. */
double Point::get_y () const {
    return y;
}


/** Returns the distance to point p from this point. */
double Point::distance (const Point& p) const {
    return sqrt(sqr(x - p.x) + sqr(y - p.y));
}


/** Returns the radius of this point (distance from the origin). */
double Point::radius () const {
    return sqrt(x*x + y*y);
}


/** Returns the angle of the polar coordinate. */
double Point::angle () const {
    if (x == 0 and y == 0) return 0;
    else return atan(x/y);
}

/**Return the slope of the line between two points. */
double Point::slope(const Point& p) const {
    return (p.get_y() - y) / (p.get_x() - x);
}

bool Point::leftof(const Point& p2, const Point& p3) const {
    return (p3.get_x() - p2.get_x()) * (y - p2.get_y()) >=
           (p3.get_y() - p2.get_y()) * (y - p2.get_x());
}

/** Compares this point to point p. */
bool Point::operator== (const Point& p) const {
    return x == p.x and y == p.y;
}


/** Compares this point to point p. */
bool Point::operator!= (const Point& p) const {
    return not operator==(p);
}


/** Adds the coordinates of p to this point and returns this point. */
Point& Point::operator+= (const Point& p) {
    x += p.x;
    y += p.y;
    return *this;
}


/** Returns the point resulting of adding the coordinates of this point and p. */
Point Point::operator+ (const Point& p) const {
    Point q = p;
    q += *this;
    return q;

    // Equivalent one-liner: return Point(p) += *this;
}
