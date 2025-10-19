#ifndef SNAKE_H
#define SNAKE_H

#include <stdbool.h>
#include <stddef.h>

typedef enum Direction {
	NORTH,
	WEST,
	EAST,
	SOUTH
} Direction;

typedef struct Point {
	unsigned int x;
	unsigned int y;
} Point;

typedef struct PointList {
	Point point;
	struct PointList *next;
} PointList;

typedef struct Snake {
	PointList *head;
	PointList *tail;
	size_t length;
	Direction moving_direction;
} Snake;

typedef struct Map {
	Point fruit;
	Snake *snake;
	bool fruit_eaten;
	unsigned int height;
	unsigned int width;
} Map;

Snake *create_snake(unsigned int x, unsigned int y, Direction direction);

Map *create_map(unsigned int width, unsigned int height);

/* Attempts to move the snake.
 * Returns true if the snake sucessfully moved,
 * false for game over.
 */
bool move_snake(Map *map);

/* Frees the snake */
void free_snake(Snake *snake);

void free_map(Map *map);

#endif /* SNAKE_H */
