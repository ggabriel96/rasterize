all: rasterize.o

rasterize.o: rasterize.cpp
	g++ rasterize.cpp -o rasterize.o -Wall -O2 -std=c++11

clean:
	rm rasterize.o
