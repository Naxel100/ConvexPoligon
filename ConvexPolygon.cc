#include "ConvexPolygon.hh"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cassert>
using namespace std;

ConvexPolygon::ConvexPolygon(vector<Point>& points, double r, double g, double b) {
    polygon = convex_hull(points);
    color.red = r;
    color.green = g;
    color.blue = b;
}

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

vector<Point> ConvexPolygon::convex_hull(vector<Point>& points) {
    if(points.size() < 3) return points;
    sort_by_slope(points);
    int size = points.size();
    vector<Point> convex_polygon = {points[0], points[1], points[2]};
    int j = 2;
    for(int i = 3; i < size; ++i) {
        while(points[i].leftof(convex_polygon[j], convex_polygon[j - 1])) {
            --j;
            convex_polygon.pop_back();
        }
        ++j;
        convex_polygon.push_back(points[i]);
    }
    return convex_polygon;
}

void ConvexPolygon::sort_by_slope(vector<Point>& points) {
    Sort_slope sort_slope;
    sort_slope.p0 = most_left_point(points);
    sort(points.begin(), points.end(), sort_slope);
}

Point ConvexPolygon::most_left_point(const vector<Point>& points) {
    Point p0 = points[0];
    for(Point p : points) {
        if(p.get_x() < p0.get_x() and p.get_y() < p0.get_y()) p0 = p;
    }
    return p0;
}

int ConvexPolygon::vertices() const {
    return polygon.size();
}

int ConvexPolygon::edges() const {
    int num_vertices = vertices();
    if(num_vertices == 1 or num_vertices == 2) return num_vertices - 1;
    return num_vertices;
}

double ConvexPolygon::area() const {
    int size = vertices();
    if(size < 3) return 0;
    double sum = 0;
    for(int i = 0; i < size; ++i) {
        sum += polygon[i].get_x() * polygon[(i + 1) % size].get_y()
             - polygon[i].get_y() * polygon[(i + 1) % size].get_x();
    }
    return sum/2; //mirar si ponerlo negativo o positivo
}

double ConvexPolygon::perimeter() const {
    int size = vertices();
    if(size < 2) return 0;
    double perimeter = polygon[0].distance(polygon.back());
    for(int i = 0; i < size - 1; ++i) perimeter += polygon[i].distance(polygon[i + 1]);
    return perimeter;
}

vector<Point> ConvexPolygon::get_points() const {
    return polygon;
}

bool ConvexPolygon::regular() const {
    if(polygon.size() < 3) return true;
    double dist0 = polygon[0].distance(polygon.back());
    int size = vertices();
    for(int i = 0; i < size - 1; ++i) {
        if(polygon[i].distance(polygon[i + 1]) != dist0) return false;
    }
    return true;
}

void ConvexPolygon::setcol(double r, double g, double b) {
    color.red = r;
    color.green = g;
    color.blue = b;
}
