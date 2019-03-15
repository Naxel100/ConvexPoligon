#ifndef ConvexPolygon_hh
#define ConvexPolygon_hh

#include <vector>
#include "Point.hh"
using namespace std;

struct Color {
    double red, green, blue;
};

class ConvexPolygon {

public:

    //Constructor
    ConvexPolygon(vector<Point>& polygon, double red = 0, double green = 0, double blue = 0);

    //Returns the number of vertices
    int vertices() const;

    //Returns the number of edges
    int edges() const;

    //Returns the number of areas
    double area() const;

    //Returns the perimeter of the polygon
    double perimeter() const;

    //Returns whether the polygon is regular or another
    bool regular() const;

    //Returns the vector of points of the polygon
    vector<Point> get_points() const;

    //Returns the centroid of the polygon
    Point centroid() const;

    //Returns whether a given polygon p2 is inside the object polygon or not
    bool inside(const vector<Point>& p2) const;

    //Modifies the color of the polygon
    void setcol(double r, double g, double b);

private:

    vector<Point> polygon;

    Color color;

    vector<Point> convex_hull(vector<Point>& points);

    Point most_left_point(const vector<Point>& points);

    void sort_by_slope(vector<Point>& points);

};

#endif
