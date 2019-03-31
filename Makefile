CXXFLAGS = -Wall -std=c++11 -O2 -DNO_FREETYPE -I $(HOME)/libs/include 

all: program.exe

program.exe: main.o Point.o ConvexPolygon.o
	$(CXX) $^ -o $@ -L $(HOME)/libs/lib -l PNGwriter -l png

main.o: main.cc Point.hh ConvexPolygon.cc ConvexPolygon.hh

Point.o: Point.cc Point.hh

ConvexPolygon.o: ConvexPolygon.cc ConvexPolygon.hh Point.hh

clean:
	rm -f program.exe *.o *.png

