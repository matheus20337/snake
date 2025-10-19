#ifndef MAP_H
#define MAP_H

typedef struct Point {
	int x;
	int y;
} Point;

typedef struct Map {
	Point fruit;
	unsigned int height;
	unsigned int width;
} Map;

Map createMap(unsigned int width, unsigned int height);

#endif
