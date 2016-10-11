all: rasterize

rasterize: rasterize.cpp
	g++ rasterize.cpp -o rasterize -Wall -O2 -std=c++11

clean:
	rm rasterize
