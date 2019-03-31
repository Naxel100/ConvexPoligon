#include "ConvexPolygon.hh"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <pngwriter.h>
#include <vector>
using namespace std;



/************************************* AUXILIAR FUNCTIONS *************************************/

// This struct is used to sort some given points in order of slope respect to a pivot point.
// INDICATION: if the slope of two points is equal the minor distance point has priority.
struct Sort_slope {
    Point p0;
    bool operator() (const Point& p1, const Point& p2) {
        if(p1.get_x() == p0.get_x() and p2.get_x() == p0.get_x()) return p1.get_y() < p2.get_y();
        if(p1.get_x() == p0.get_x()) return true;
        if(p2.get_x() == p0.get_x()) return false;
        if(p1.slope(p0) != p2.slope(p0)) return p1.slope(p0) > p2.slope(p0);
        return p1.distance(p0) < p2.distance(p0);
    }
};

// Returns whether a number "a" is found between a number "b" and a number "c".
bool between(double a, double b, double c) {
    return a >= min(b, c) and a <= max(b, c);
}

// This void adds to a given vector of points the intersection of two segments if it exists.
void intersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2, vector<Point>& intersection) {
    // Line equation of the segment (p1, p2).
    double a1 = p2.get_x() - p1.get_x();
    double b1 = p1.get_y() - p2.get_y();
    double c1 = p2.get_x() * p1.get_y() - p2.get_y() * p1.get_x();
    // Line equation of the segment (q1, q2).
    double a2 = q2.get_x() - q1.get_x();
    double b2 = q1.get_y() - q2.get_y();
    double c2 = q2.get_x() * q1.get_y() - q2.get_y() * q1.get_x();
    // Solve the equation system with Cramer
    double x, y;
    double det = a1 * b2 - a2 * b1;
    if(det == 0) return;
    else {
        x = (a1 * c2 - a2 * c1) / det;
        y = (c1 * b2 - c2 * b1) / det;
    }
    //
    ConvexPolygon line1({p1, p2}), line2({q1, q2}), polygon_point({Point(x, y)});
    if(polygon_point.inside(line1) and polygon_point.inside(line2)) intersection.push_back(Point(x, y)); //Checks if the point is inside the segments.
}



/***************************** FUNCTIONS FROM THE CONVEXPOLYGON CLASS *****************************/

// Constructor
ConvexPolygon::ConvexPolygon(const vector<Point>& points, const Color& entry_color) {
    polygon = points;
    color.red = entry_color.red;
    color.green = entry_color.green;
    color.blue = entry_color.blue;
    convex_hull();
}

// This function returns the bounding box of a given polygon
ConvexPolygon ConvexPolygon::bbox() const {
    if(vertices() <= 1) return *this; // We consider that the boundix box of a point or the empty polygon is the same polybon
    double min_x, max_x, min_y, max_y;
    min_x = max_x = polygon[0].get_x();
    min_y = max_y = polygon[0].get_y();
    for(Point point : polygon) {
        if(point.get_x() < min_x) min_x = point.get_x();
        if(point.get_x() > max_x) max_x = point.get_x();
        if(point.get_y() > max_y) max_y = point.get_y();
        if(point.get_y() < min_y) min_y = point.get_y();
    }
    Point p1(min_x, min_y);
    Point p2(min_x, max_y);
    Point p3(max_x, max_y);
    Point p4(max_x, min_y);
    return ConvexPolygon({p1, p2, p3, p4});
}

// This operator returns the intersection polygon of this polygon and the polygon p2.
ConvexPolygon ConvexPolygon::operator* (const ConvexPolygon& p2) const {
    vector<Point> intersection;
    int size1 = vertices(), size2 = p2.vertices();
    // Let's include in the intersection all the points from each polygon which are inside the other one.
    push_points_inside(p2, intersection);
    p2.push_points_inside(*this, intersection);
    //
    for(int i = 0; i < size1; ++i) {
        Point a1 = polygon[i], a2 = polygon[(i + 1) % size1];
        for(int j = 0; j < size2; ++j) {
            Point b1 = p2.get_points()[j], b2 = p2.get_points()[(j + 1) % size2];
            intersect(a1, a2, b1, b2, intersection);
        }
    }
    return ConvexPolygon({intersection});
}

// This operator returns the union polygon of this polygon and the polygon p2.
ConvexPolygon ConvexPolygon::operator+ (const ConvexPolygon& p2) const {
    vector<Point> unio = polygon;
    vector<Point> points2 = p2.get_points();
    unio.insert(unio.end(), points2.begin(), points2.end());
    return ConvexPolygon(unio);
}

// Returns a point which represents the centroid of a given polygon.
// For more information about the computation and centroid's formula: https://en.wikipedia.org/wiki/Centroid
Point ConvexPolygon::centroid() const {
    int size = vertices();
    double coordinate_x = 0, coordinate_y = 0;
    for(int i = 0; i < size; ++i) {
        double det = polygon[i].get_y() * polygon[(i + 1) % size].get_x()
                   - polygon[i].get_x() * polygon[(i + 1) % size].get_y();
        coordinate_x += (polygon[i].get_x() + polygon[(i + 1) % size].get_x()) * det;
        coordinate_y += (polygon[i].get_y() + polygon[(i + 1) % size].get_y()) * det;
    }
    assert(area() != 0);
    double centroid_x = coordinate_x / (6 * area());
    double centroid_y = coordinate_y / (6 * area());
    if(abs(centroid_x) < epsilon) centroid_x = 0;
    if(abs(centroid_y) < epsilon) centroid_y = 0;
    return {centroid_x, centroid_y};
}

// As the name indicates this function returns the most left point of a polygon.
// INDICATION: In case of tie, the minor 'y' coordinate has priority.
Point ConvexPolygon::most_left_point() {
    assert(not polygon.empty());
    Point p0 = polygon[0];
    for(Point p : polygon) {
        if(p.get_x() < p0.get_x()) p0 = p;
    }
    return p0;
}

// Returns a vector of polygons whose points are scaled to enter and occupy the maximum size of the png image.
// INDICATION: The maximum size of this represents 496 pixels.
vector<ConvexPolygon> ConvexPolygon::scale(const vector<ConvexPolygon>& polygons) const {
    assert(polygon.size()>= 3);
    double height = polygon[0].distance(polygon[1]);
    double width = polygon[1].distance(polygon[2]);
    double scale_factor = 496 / max(height, width);
    vector<ConvexPolygon> result;
    for(ConvexPolygon convex_polygon : polygons) {
        vector<Point> scaled_points;
        for(Point point : convex_polygon.get_points()) {
            // Scale each point and move it in order to center the polygons image in the png.
            double x = (point.get_x() - polygon[0].get_x()) * scale_factor + 251 - (centroid().get_x() - polygon[0].get_x()) * scale_factor;
            double y = (point.get_y() - polygon[0].get_y()) * scale_factor + 251 - (centroid().get_y() - polygon[0].get_y()) * scale_factor;
            scaled_points.push_back({x, y});
        }
        result.push_back(ConvexPolygon(scaled_points, {convex_polygon.get_red(), convex_polygon.get_green(), convex_polygon.get_blue()}));
    }
    return result;
}

// Returns the points' vector of a this polygon.
vector<Point> ConvexPolygon::get_points() const {
    return polygon;
}

// This function, given a polygon, returns a points' vectors with all the polygon's points but without repetitions.
vector<Point> ConvexPolygon::remove_repited_points() {
    vector<Point> removed_points;
    int size = vertices();
    for(int i = 0; i < size; ++i) {
        removed_points.push_back(polygon[i]);
        while(i + 1 < size and polygon[i] == polygon[i + 1]) ++i;
    }
    return removed_points;
}

// Returns the number of vertices of this polygon.
int ConvexPolygon::vertices() const {
    return polygon.size();
}

// Returns the number of edges of this polygon.
int ConvexPolygon::edges() const {
    int num_vertices = vertices();
    if(num_vertices < 3) return num_vertices - 1;
    return num_vertices;
}

// Returns the area of a gven polygon.
// INDICATION: if the number of vertices of vertices of the polygon is < 3 the area is 0.
// For more information of computation and area's polygon formula: https://www.universoformulas.com/matematicas/geometria/area-poligono-irregular/
double ConvexPolygon::area() const {
    int size = vertices();
    if(size < 3) return 0;
    double sum = 0;
    for(int i = 0; i < size; ++i) {
        sum += polygon[i].get_x() * polygon[(i + 1) % size].get_y()
             - polygon[i].get_y() * polygon[(i + 1) % size].get_x();
    }
    return -sum/2;
}

// Returns the polygon's red color quantity.
double ConvexPolygon::get_red() const {
    return color.red;
}

// Returns the polygon's green color quantity.
double ConvexPolygon::get_green() const {
    return color.green;
}

// Returns the polygon's blue color quantity.
double ConvexPolygon::get_blue() const {
    return color.blue;
}

// Returns the perimeter of this polygon.
// INDICATION: if the polygon's size is < 2 the perimeter is 0.
double ConvexPolygon::perimeter() const {
    int size = vertices();
    if(size < 2) return 0;
    double perimeter = polygon[0].distance(polygon.back());
    for(int i = 0; i < size - 1; ++i) perimeter += polygon[i].distance(polygon[i + 1]);
    return perimeter;
}

// By an algorithm of divide and conquer this function returns whether a point is inside this polygon or not.
bool ConvexPolygon::has_inside(const Point& point) const {
    assert(vertices() >= 3);
    int start = 1;
    int end = vertices() - 1;
    while(end - start != 1) {
        int middle = (start + end) / 2;
        if(not point.rightof(polygon[0], polygon[middle])) end = middle;
        else start = middle;
    }
    return point.rightof(polygon[0], polygon[start]) and point.rightof(polygon[start], polygon[end]) and
           point.rightof(polygon[end], polygon[0]);
}

// Returns whether a polygon p2 is inside of a given polygon.
bool ConvexPolygon::inside(const ConvexPolygon& p2) const {
    if(p2.vertices() == 0 or vertices() == 0) return false;
    if(p2.vertices() == 1) {
        if(polygon.size() != 1) return false;
        return polygon[0] == p2.polygon[0];
    }
    if(p2.vertices() == 2) {
        if(polygon.size() != 1) return false;
        Point a = p2.get_points()[0], b = p2.get_points()[1], c = polygon[0];
        return c.leftof(a, b) and c.rightof(a, b) and between(c.get_x(), a.get_x(), b.get_x()) and between(c.get_y(), a.get_y(), b.get_y());
    }
    for(Point point : get_points()) {
        if(not p2.has_inside(point)) return false;
    }
    return true;
}

// Returns whether this polygon is regular or not.
// INDICATION: all polygons whose size is < 3 are considered regular, including the empty polyon.
bool ConvexPolygon::regular() const {
    if(polygon.size() < 3) return true;
    double dist0 = polygon[0].distance(polygon.back());
    int size = vertices();
    for(int i = 0; i < size - 1; ++i) {
        if(polygon[i].distance(polygon[i + 1]) != dist0) return false;
    }
    return true;
}

// This action executes the convex hull algorithm to a given polygon.
void ConvexPolygon::convex_hull() {
    sort_by_slope();
    polygon = remove_repited_points();
    int size = vertices();
    if(size < 3) return;
    vector<Point> convex_polygon = {polygon[0], polygon[1]};
    int j = 1;
    for(int i = 2; i < size; ++i) {
        while(polygon[i].leftof(convex_polygon[j - 1], convex_polygon[j]) and j > 0) {
            --j;
            convex_polygon.pop_back();
        }
        ++j;
        convex_polygon.push_back(polygon[i]);
    }
    polygon = convex_polygon;
}

// Given a polygons' vector this action creates a png file whith a given name and draws in it all the polygons from the vector who are inside this polygon.
void ConvexPolygon::draw(const string& file_name, const vector<ConvexPolygon>& polygons) const {
    const char * file_name_pointer = file_name.c_str();
    pngwriter png(501, 501, 1.0, file_name_pointer);
    if(vertices() == 1) {
        // If the bounding box is a point, it means that we only want to draw a point. Thus we paint a point in the center of the png image.
        png.line(251, 251, 251, 251, polygons[0].get_red(), polygons[0].get_green(), polygons[0].get_blue());
        png.close();
        return;
    }
    vector<ConvexPolygon> copy;
    if(vertices() > 3) copy = scale(polygons);
    for(ConvexPolygon cp : copy) {
        vector<Point> v = cp.get_points();
        int size = v.size();
        for(int i = 0; i < size - 1; ++i) {
            // In this iterator we draw all the lines but the last one.
            png.line(int(v[i].get_x()), int(v[i].get_y()), int(v[i + 1].get_x()), int(v[i + 1].get_y()), cp.get_red(), cp.get_green(), cp.get_blue());
        }
        // Let's draw the last polygon's line.
        if(not size == 0) png.line(int(v[size - 1].get_x()), int(v[size - 1].get_y()), int(v[0].get_x()), int(v[0].get_y()), cp.get_red(), cp.get_green(), cp.get_blue());
    }
    png.close();
}

// This action inserts in the intersection vector all the points from the given polygon which are inside the polygon p2.
void ConvexPolygon::push_points_inside(const ConvexPolygon& p2, vector<Point>& intersection) const {
    for(Point point : polygon) {
        ConvexPolygon polygon_point({point});
        if(polygon_point.inside(p2)) intersection.push_back(point);
    }
}

// This action changes the color of this polygon for the r g b given color.
void ConvexPolygon::setcol(double r, double g, double b) {
    color.red = r;
    color.green = g;
    color.blue = b;
}

// Given a polygon the action sorts all its points by slope.
void ConvexPolygon::sort_by_slope() {
    if(vertices() < 2) return;
    Sort_slope sort_slope;
    sort_slope.p0 = most_left_point();
    sort(polygon.begin(), polygon.end(), sort_slope);
}
