CXXFLAGS = -Wall -std=c++11 -O2

all: program.exe

program.exe: main.o Point.o ConvexPolygon.o
	$(CXX) $^ -o $@

main.o: main.cc Point.hh ConvexPolygon.cc ConvexPolygon.hh

Point.o: Point.cc Point.hh

ConvexPolygon.o: ConvexPolygon.cc ConvexPolygon.hh Point.hh

clean:
	rm -f main.exe *.o

