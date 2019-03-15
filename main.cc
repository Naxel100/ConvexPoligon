#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include "Point.hh"
#include "ConvexPolygon.hh"
using namespace std;

map<string, ConvexPolygon> map_polygons;

void define_polygon(const string& elements) {
    istringstream iss(elements);
    string name;
    iss >> name;
    double x_coord, y_coord;
    vector<Point> points;
    while(iss >> x_coord >> y_coord) points.push_back(Point(x_coord, y_coord));
    map_polygons.insert({name, ConvexPolygon(points, 0, 0, 0)});
}

void print() {
    string name;
    cin >> name;
    bool primer = true;
    for(Point point : map_polygons[name].get_points()) {
        if(primer) primer = false;
        else cout << " ";
        cout << point.get_x() << " " << point.get_y();
    }
    cout << endl;
}

void perimeter(const string& elements) {
    string name;
    cin >> name;
    cout << map_polygons[name].perimeter() << endl;
}

void area(const string& elements) {
    string name;
    cin >> name;
    cout << map_polygons[name].area() << endl;
}

void vertices(const string& elements) {
    string name;
    cin >> name;
    cout << map_polygons[name].vertices() << endl;
}

void setcol(const string& elements) {
    string name;
    double red, green, blue;
    cin >> name >> red >> green >> blue;
    map_polygons[name].setcol(red, green, blue);
}

void regular(const string& elements) {
    string name;
    cin >> name;
    bool regular = map_polygons[name].regular();
    cout << (regular ? "yes" : "no") << endl;
}

void edges(const string& elements) {
    string name;
    cin >> name;
    cout << map_polygons[name].edges() << endl;
}

int main() {
    string action;
    while(cin >> action) {
        string elements;
        if(action == "perimeter") perimeter();
        else if(action == "area") area();
        else if(action == "vertices") vertices();
        else if(action == "setcol") setcol();
        else if(action == "regular") regular();
        else if(action == "edges") edges();
        else if(action == "polygon") define_polygon();
        else if(action == "print") print();
    }
}
