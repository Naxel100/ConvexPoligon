#include "ConvexPolygon.hh"
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cassert>
#include <pngwriter.h>
using namespace std;

ConvexPolygon::ConvexPolygon(const vector<Point>& points, const Color& color) {
    polygon = points;
    convex_hull();
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

vector<Point> ConvexPolygon::remove_repited_points() {
    vector<Point> removed_points;
    int size = vertices();
    for(int i = 0; i < size; ++i) {
        removed_points.push_back(polygon[i]);
        while(i + 1 < size and polygon[i] == polygon[i + 1]) ++i;
    }
    return removed_points;
}

void ConvexPolygon::sort_by_slope() {
    if(vertices() < 2) return;
    Sort_slope sort_slope;
    sort_slope.p0 = most_left_point();
    sort(polygon.begin(), polygon.end(), sort_slope);
}

Point ConvexPolygon::most_left_point() {
    Point p0 = polygon[0];
    for(Point p : polygon) {
        if(p.get_x() < p0.get_x()) p0 = p;
    }
    return p0;
}

ConvexPolygon ConvexPolygon::bbox() const {
    if(vertices() <= 1) return *this;
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

vector<ConvexPolygon> ConvexPolygon::scale(const vector<ConvexPolygon>& polygons) const {
    double height = polygon[0].distance(polygon[1]);
    double width = polygon[1].distance(polygon[2]);
    double scale_factor = max(height, width); //move_factor = min(height, width);
    vector<ConvexPolygon> result;
    for(ConvexPolygon convex_polygon : polygons) {
        vector<Point> scaled_points;
        for(Point point : convex_polygon.get_points()) {
            double x = (point.get_x() - polygon[0].get_x()) * scale_factor; // + (scale_factor - move_factor) / 2;
            double y = (point.get_y() - polygon[0].get_y()) * scale_factor; // + (scale_factor - move_factor) / 2;
            scaled_points.push_back({x, y});
        }
        result.push_back(ConvexPolygon(scaled_points));
    }
    return result;
}

void ConvexPolygon::draw(const string& file_name, const vector<ConvexPolygon>& polygons) const {
    const char * file_name_pointer = file_name.c_str();
    pngwriter png(500, 500, 1.0, file_name_pointer);
    if(vertices() == 1) {
        //png.circle(250, 250, 1, polygons[0].get_red(), polygons[0].get_green(), polygons[0].get_blue());
        //png.close();
        return;
    }
    vector<ConvexPolygon> copy;
    if(vertices() > 3) copy = scale(polygons);
    for(ConvexPolygon cp : copy) {
        vector<Point> v = cp.get_points();
        int size = v.size();
        for(int i = 0; i < size - 1; ++i) {
            //png.line(int(v[i].get_x()), int(v[i].get_y()), int(v[i + 1].get_x()), int(v[i + 1 % size].get_y()), cp.get_red(), cp.get_green(), cp.get_blue());
        }
    }
    png.close();
}


bool ConvexPolygon::has_inside(const Point& point) const {
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

void intersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2, vector<Point>& intersection) {
    //First equation
    double a1 = p2.get_x() - p1.get_x();
    double b1 = p1.get_y() - p2.get_y();
    double c1 = p2.get_x() * p1.get_y() - p2.get_y() * p1.get_x();

    //Second ecuation
    double a2 = q2.get_x() - q1.get_x();
    double b2 = q1.get_y() - q2.get_y();
    double c2 = q2.get_x() * q1.get_y() - q2.get_y() * q1.get_x();

    //Solve the equation with Cramer
    double x, y;
    double det = a1 * b2 - a2 * b1;
    if(det == 0) return;
    else {
        x = (a1 * c2 - a2 * c1) / det;
        y = (c1 * b2 - c2 * b1) / det;
    }
    ConvexPolygon line1({p1, p2}), line2({q1, q2}), polygon_point({Point(x, y)});
    if(polygon_point.inside(line1) and polygon_point.inside(line2)) intersection.push_back(Point(x, y));
}

void ConvexPolygon::push_points_inside(const ConvexPolygon& p2, vector<Point>& intersection) const {
    for(Point point : polygon) {
        ConvexPolygon polygon_point({point});
        if(polygon_point.inside(p2)) intersection.push_back(point);
    }
}

ConvexPolygon ConvexPolygon::operator* (const ConvexPolygon& p2) const {
    vector<Point> intersection;
    int size1 = vertices(), size2 = p2.vertices();
    push_points_inside(p2, intersection);
    p2.push_points_inside(*this, intersection);
    for(int i = 0; i < size1; ++i) {
        Point a1 = polygon[i], a2 = polygon[(i + 1) % size1];
        for(int j = 0; j < size2; ++j) {
            Point b1 = p2.get_points()[j], b2 = p2.get_points()[(j + 1) % size2];
            intersect(a1, a2, b1, b2, intersection);
        }
    }
    return ConvexPolygon({intersection});
}

bool between(double a, double b, double c) {
    return a >= min(b, c) and a <= max(b, c);
}

bool ConvexPolygon::inside(const ConvexPolygon& p2) const {
    if(p2.vertices() == 0) return false;
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

int ConvexPolygon::vertices() const {
    return polygon.size();
}

int ConvexPolygon::edges() const {
    int num_vertices = vertices();
    if(num_vertices < 3) return num_vertices - 1;
    return num_vertices;
}

Point ConvexPolygon::centroid() const {
    int size = vertices();
    double coordinate_x = 0, coordinate_y = 0;
    for(int i = 0; i < size; ++i) {
        double det = polygon[i].get_y() * polygon[(i + 1) % size].get_x()
                   - polygon[i].get_x() * polygon[(i + 1) % size].get_y();
        coordinate_x += (polygon[i].get_x() + polygon[(i + 1) % size].get_x()) * det;
        coordinate_y += (polygon[i].get_y() + polygon[(i + 1) % size].get_y()) * det;
    }
    double centroid_x = coordinate_x / (6 * area());
    double centroid_y = coordinate_y / (6 * area());
    if(abs(centroid_x) < epsilon) centroid_x = 0;
    if(abs(centroid_y) < epsilon) centroid_y = 0;
    return {centroid_x, centroid_y};
}

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

double ConvexPolygon::get_red() const {
    return color.red;
}

double ConvexPolygon::get_green() const {
    return color.green;
}

double ConvexPolygon::get_blue() const {
    return color.blue;
}

ConvexPolygon ConvexPolygon::operator+ (const ConvexPolygon& p2) const {
    vector<Point> unio = polygon;
    vector<Point> points2 = p2.get_points();
    unio.insert(unio.end(), points2.begin(), points2.end());
    return ConvexPolygon(unio);
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
