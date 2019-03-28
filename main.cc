#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <pngwriter.h>
#include "Point.hh"
#include "ConvexPolygon.hh"
using namespace std;

map<string, ConvexPolygon> map_polygons;

bool inexistent(const string& name) {
    return map_polygons.find(name) == map_polygons.end();
}

void area(istringstream& iss) {
    string name;
    iss >> name;
    if(inexistent(name)) cout << "error: undefined identifier." << endl;
    else cout << map_polygons[name].area() << endl;
}

void bbox(istringstream& iss) {
    string name, polygon;
    iss >> name;
    ConvexPolygon result;
    while(iss >> polygon) {
        if(inexistent(polygon)) cout << "warning: the given polygon " << polygon << " doesn't exist." << endl;
        else result = result + map_polygons[polygon];
    }
    if(name.empty()) cout << "error: a polygon name is expected." << endl;
    else {
        map_polygons.insert({name, result.bbox()});
        cout << "ok" << endl;
    }
}

void centroid(istringstream& iss) {
    string name;
    iss >> name;
    if(inexistent(name)) cout << "error: undefined identifier." << endl;
    else if(map_polygons[name].vertices() >= 3) cout << map_polygons[name].centroid().get_x() << " " << map_polygons[name].centroid().get_y() << endl;
    else cout << "error: the polygon " << name << " has too few points to compute the centroid." << endl;
}

void color(istringstream& iss) {
    string name;
    iss >> name;
    if(inexistent(name)) cout << "error: undefined identifier." << endl;
    else cout << "red: " << map_polygons[name].get_red() << "  green: " << map_polygons[name].get_green() << "  blue: " << map_polygons[name].get_blue() << endl;
}

void define_polygon(istringstream& iss) {
    string name;
    iss >> name;
    double x_coord, y_coord;
    vector<Point> points;
    while(iss >> x_coord) {
        if(iss >> y_coord) points.push_back(Point(x_coord, y_coord));
        else cout << "One value is missing, the last number won't be saved." << endl;
    }
    if(name.empty()) cout << "error: a polygon name is expected." << endl;
    else {
        cout << "ok" << endl;
        map_polygons.insert({name, ConvexPolygon(points)});
    }
}

void draw(istringstream& iss) {
    string file_name;
    iss >> file_name;
    string polygon;
    vector<ConvexPolygon> polygons;
    ConvexPolygon great_polygon;
    while(iss >> polygon) {
        if(inexistent(polygon)) cout << "warning: the given polygon " << polygon << " doesn't exist." << endl;
        else {
            polygons.push_back(map_polygons[polygon]);
            great_polygon = great_polygon + map_polygons[polygon];
        }
    }
    great_polygon = great_polygon.bbox();
    great_polygon.draw(file_name, polygons);
}

void edges(istringstream& iss) {
    string name;
    iss >> name;
    if(inexistent(name)) cout << "error: undefined identifier." << endl;
    else cout << map_polygons[name].edges() << endl;
}

void inside(istringstream& iss) {
    string polygon1, polygon2;
    iss >> polygon1 >> polygon2;
    if(polygon1.empty() or polygon2.empty()) cout << "error: two polygons are expected." << endl;
    else if(inexistent(polygon1)) cout << "error: the given polygon " << polygon1 << " doesn't exist." << endl;
    else if(inexistent(polygon2)) cout << "error: the given polygon " << polygon2 << " doesn't exist." << endl;
    else cout << (map_polygons[polygon1].inside(map_polygons[polygon2]) ? "yes" : "no") << endl;
}

void intersection(istringstream& iss) {
    string polygon;
    vector<string> polygons;
    bool first = true;
    while(iss >> polygon) {
        if(not first and inexistent(polygon)) cout << "warning: the given polygon " << polygon << " doesn't exist." << endl;
        else polygons.push_back(polygon);
    }
    if(polygons.size() != 2 and polygons.size() != 3) {
        cout << "error: incorrect number of given polygons." << endl;
        return;
    }
    else if(polygons.size() == 3) map_polygons.insert({polygons[0], map_polygons[polygons[1]] * map_polygons[polygons[2]]});
    else map_polygons[polygons[0]] = map_polygons[polygons[0]] * map_polygons[polygons[1]];
    cout << "ok" << endl;
}

void list() {
    bool primer = true;
    for(auto polygons : map_polygons) {
        if(primer) primer = false;
        else cout << " ";
        cout << polygons.first;
    }
    cout << endl;
}

void load(istringstream& iss) {
    string file_name;
    iss >> file_name;
    ifstream ifs(file_name);

    string line;
    while(getline(ifs, line)) {
        istringstream iss_file(line);
        string name;
        iss_file >> name;
        double x_coord, y_coord;
        vector<Point> points;
        while(iss >> x_coord) {
            if(iss >> y_coord) points.push_back(Point(x_coord, y_coord));
            else cout << "One value is missing, the last number won't be saved." << endl;
        }
        map_polygons.insert({name, ConvexPolygon(points)});
    }
    if(file_name.empty()) cout << "error: a file name is expected." << endl;
    else cout << "ok" << endl;
    ifs.close();
}

void perimeter(istringstream& iss) {
    string name;
    iss >> name;
    if(inexistent(name)) cout << "error: undefined identifier." << endl;
    else cout << map_polygons[name].perimeter() << endl;
}

void print(istringstream& iss) {
    string name;
    iss >> name;
    if(inexistent(name)) {
        cout << "error: undefined identifier." << endl;
        return;
    }
    bool primer = true;
    for(Point point : map_polygons[name].get_points()) {
        if(primer) primer = false;
        else cout << "   ";
        cout << point.get_x() << " " << point.get_y();
    }
    cout << endl;
}

void regular(istringstream& iss) {
    string name;
    iss >> name;
    if(inexistent(name)) cout << "error: undefined identifier." << endl;
    else {
        bool regular = map_polygons[name].regular();
        cout << (regular ? "yes" : "no") << endl;
    }
}

void save(istringstream& iss) {
    string file_name;
    iss >> file_name;
    ofstream ofs(file_name);

    string polygon;
    while(iss >> polygon) {
        if(inexistent(polygon)) {
            cout << "warning: the polygon " << polygon << " doesn't exist and won't be saved." << endl;
            return;
        }
        ofs << polygon;
        vector<Point> points = map_polygons[polygon].get_points();
        for(Point point : points) ofs << "  " << point.get_x() << " " << point.get_y();
        ofs << endl;
    }
    if(file_name.empty()) cout << "error: a file name is expected." << endl;
    else cout << "ok" << endl;
    ofs.close();
}

void setcol(istringstream& iss) {
    string name;
    double red, green, blue;
    iss >> name >> red >> green >> blue;
    if(inexistent(name)) cout << "error: undefined identifier." << endl;
    else {
        if(red < 0 or red > 1 or green < 0 or green > 1 or blue < 0 or blue > 1) cout << "error: all entries must be between 0 and 1, included." << endl;
        else map_polygons[name].setcol(red, green, blue);
    }
    cout << "ok" << endl;
}

void unio(istringstream& iss) {
    string polygon;
    vector<string> polygons;
    while(iss >> polygon) {
        if(inexistent(polygon)) cout << "warning: the given polygon " << polygon << " doesn't exist." << endl;
        else polygons.push_back(polygon);
    }
    if(polygons.size() != 2 and polygons.size() != 3) {
        cout << "error: incorrect number of given polygons." << endl;
        return;
    }
    else if(polygons.size() == 3) map_polygons.insert({polygons[0], map_polygons[polygons[1]] + map_polygons[polygons[2]]});
    else map_polygons[polygons[0]] = map_polygons[polygons[0]] + map_polygons[polygons[1]];
    cout << "ok" << endl;
}

void vertices(istringstream& iss) {
    string name;
    iss >> name;
    if(inexistent(name)) cout << "error: undefined identifier." << endl;
    else cout << map_polygons[name].vertices() << endl;
}

int main() {
    cout.setf(ios::fixed);
    cout.precision(3);

    string line;
    while(getline(cin, line)) {
        istringstream iss(line);
        string action;
        iss >> action;
        if(action == "area") area(iss);
        else if(action == "bbox") bbox(iss);
        else if(action == "centroid") centroid(iss);
        else if(action == "color") color(iss);
        //else if(action == "draw") draw(iss);
        else if(action == "edges") edges(iss);
        else if(action == "inside") inside(iss);
        else if(action == "intersection") intersection(iss);
        else if(action == "list") list();
        else if(action == "load") load(iss);
        else if(action == "perimeter") perimeter(iss);
        else if(action == "polygon") define_polygon(iss);
        else if(action == "print") print(iss);
        else if(action == "regular") regular(iss);
        else if(action == "save") save(iss);
        else if(action == "setcol") setcol(iss);
        else if(action == "union") unio(iss);
        else if(action == "vertices") vertices(iss);
        else if(action == "#") cout << "#" << endl;
        else cout << "error: the command " << action << " can't be recognised." << endl;
    }
}
