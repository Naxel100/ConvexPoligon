#ifndef ConvexPolygon_hh
#define ConvexPolygon_hh

#include <vector>
#include <string>
#include "Point.hh"
using namespace std;

struct Color {
    double red, green, blue;
};

class ConvexPolygon {

public:

    //Constructor
    ConvexPolygon(const vector<Point>& polygon = {}, const Color& color = {0, 0, 0});

    ConvexPolygon bbox() const;

    ConvexPolygon operator+ (const ConvexPolygon& p2) const;

    ConvexPolygon operator* (const ConvexPolygon& p2) const;

    //Returns the centroid of the polygon
    Point centroid() const;

    vector<ConvexPolygon> scale(const vector<ConvexPolygon>& polygons) const;

    //Returns the vector of points of the polygon
    vector<Point> get_points() const;

    //Returns the number of edges
    int edges() const;

    //Returns the number of vertices
    int vertices() const;

    //Returns the number of areas
    double area() const;

    double get_blue() const;

    double get_green() const;

    double get_red() const;

    //Returns the perimeter of the polygon
    double perimeter() const;

    //Returns whether the polygon is regular or another
    bool regular() const;

    bool inside(const ConvexPolygon& p2) const;

    //Returns whether a given polygon p2 is inside the object polygon or not
    bool inside(const vector<Point>& p2) const;

    void draw(const string& file_name, const vector<ConvexPolygon>& polygons) const;

    //Modifies the color of the polygon
    void setcol(double r, double g, double b);

private:

    vector<Point> polygon;

    Color color;

    Point most_left_point();

    vector<Point> remove_repited_points();

    bool has_inside(const Point& point) const;

    void convex_hull();

    void push_points_inside(const ConvexPolygon& p2, vector<Point>& intersection) const;

    void sort_by_slope();
};

#endif
