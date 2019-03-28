#include "Point.hh"
#include <iostream>
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

bool Point::leftof(const Point& p1, const Point& p2) const {
    double val = (p2.get_x() - p1.get_x()) * (y - p1.get_y()) -
                 (p2.get_y() - p1.get_y()) * (x - p1.get_x());
    return val >= 0 or abs(val) < epsilon;
}

bool Point::rightof(const Point& p1, const Point& p2) const {
    double val = (p2.get_x() - p1.get_x()) * (y - p1.get_y()) -
                 (p2.get_y() - p1.get_y()) * (x - p1.get_x());
    return val <= 0 or abs(val) < epsilon;
}

/*bool Point::inside_triangle(const vector<Point>& points, int i, int j) const {
    for(int k = i; k < j; ++k) {
        if(leftof(points[k], points[k + 1])) return false;
    }
    return true;
}

bool Point::recursive_inside(const vector<Point>& points, int i, int j) const {
    if(i - j <= 2) return inside_triangle(points, i, j);
    int m = (i + j) / 2 + 1;
    return recursive_inside(points, i, m) or recursive_inside(points, m, j) or inside_triangle({points[i], points[m], points[j]}, 0, 2);
}

bool Point::is_inside(const vector<Point>& points) const {
    if(points.size() == 0) return false;
    if(points.size() == 1) return *this == points[0];
    if(points.size() == 2) {
        double lamda1 = (get_x() - points[0].get_x()) / (points[1].get_x() - points[0].get_x());
        double lamda2 = (get_y() - points[0].get_y()) / (points[1].get_y() - points[0].get_y());
        return lamda1 == lamda2 and lamda1 >= 0 and lamda1 <= 1;
    }
    return recursive_inside(points, 0, points.size() - 1);
}*/

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
