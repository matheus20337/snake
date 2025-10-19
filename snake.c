#include "snake.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

static bool position_in_snake(Point point, const Snake *snake) {
	for (PointList *i = snake->tail; i != NULL; i = i->next) {
		if (point.x == i->point.x && point.y == i->point.y) {
			return true;
		}
	}
	return false;
}

static bool point_out_of_bounds(Point point, const Map* map) {
	if (point.x >= map->width || point.y >= map->height)
		return true;
	return false;
}

static void spawn_fruit(Map *map) {
	if (map->snake->length >= map->width * map->height) return;

	Point point;
	do {
		point.x = rand() % map->width;
		point.y = rand() % map->height;
	} while(position_in_snake(point, map->snake));

	map->fruit.x = point.x;
	map->fruit.y = point.y;
	map->fruit_eaten = false;
}

void free_snake(Snake *snake) {
	while(snake->tail != NULL) {
		PointList *tmp = snake->tail;
		snake->tail = snake->tail->next;
		free(tmp);
	}
	free(snake);
}

/*⠀⠀⠀⢘⠀⡂⢠⠆⠀⡰⠀⡀⢀⣠⣶⣦⣶⣶⣶⣶⣾⣿⣿⡿⢀⠈⢐⠈⠀⠀
 *⠀⠀⠀⡃⠀⡀⣞⡇⢰⠃⣼⣇⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⠛⣰⣻⡀⢸⠀⠀⠀
 *⠀⠀⢐⢀⠀⣛⣽⣇⠘⢸⣿⣿⣷⣾⣿⣿⣿⣿⣿⣿⠟⢡⣾⣿⢿⡇⠀⡃⠀⠀
 *⠀⠀⢐⠀⠀⢳⣿⡯⡞⣾⣿⣿⣿⣿⣿⣿⢿⣿⠟⢁⣴⣿⣿⣿⡜⢷⠀⢘⠄⠀
 *⠀⠀⠂⡂⠸⡆⠙⠛⡵⣿⣿⣿⣿⣿⡿⠤⠛⣠⣴⣿⣿⠿⣟⣟⠟⢿⡆⢳⠀⠀
 *⠀⠀⠸⠁⠀⡾⠁⠀⠀⠀⠀⠉⠉⠉⠈⣠⡌⢁⠄⡛⠡⠉⠍⠙⢳⢾⠁⢸⠀⠀
 *⠀⠀⢈⠀⢀⠌⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣷⡎⠙⢬⣳⣪⡯⢜⣷⢸⠂⡈⠄⠀
 *⠀⠀⠐⡀⠀⢣⠀⠀⠀⠀⠀⠀⠀⣴⣿⣾⣷⢿⢻⣅⣌⡯⢛⣿⣿⡞⠠⡁⠂⠀
 *⠀⠀⠀⠄⠀⢉⡀⠀⠀⢀⡠⠤⠼⣇⣳⣿⣿⣟⡜⣿⣿⣿⣿⣿⣿⡇⠸⠡⠀⠀
 *⠀⠀⡀⠅⠀⠃⢀⡀⣿⡹⠗⢀⠛⠥⣺⣿⣿⡝⢹⣸⣿⣿⣿⣿⡏⠠⠰⠈⠐⠀
 *⠠⠈⠀⠄⣀⠀⠀⠸⠻⠦⠀⠀⠀⠀⠀⠉⠐⠀⠘⠻⢹⣿⡿⠃⠀⡀⠕⣈⠡⡄
 *⠀⠀⣴⡀⣬⠁⠀⠀⡁⠂⠀⣀⣀⠔⠌⠤⣀⡀⠀⠀⡈⢸⠪⠀⠀⡌⠤⠈⡀⣠
 *⠀⠀⣿⣿⣾⡇⠀⠀⠀⣴⢫⣾⠃⠠⢰⣶⣴⠶⣿⣦⠀⠀⠀⢄⣂⠀⠀⠰⠀⠙
 *⠀⠀⠉⠛⠛⠀⢀⣴⣿⢗⡟⠡⣄⣀⡀⠀⢀⣤⠞⡅⠀⠁⠀⡾⠀⠀⠠⡗⠀⢀
 *⠀⠀⠀⠀⠀⣴⡿⢋⠔⠃⠀⠀⠍⠙⠉⠈⠑⠁⠂⠀⠀⠀⡡⡁⣠⡼⣸⠅⠀⠘
 *⠀⠀⠀⣼⠛⢡⠔⠁⠐⣆⠀⠀⠀⠀⠀⠀⠀⠀⠁⢀⡔⡞⢛⣿⡿⠃⠏⠀⠀⢠
 *⠀⠀⠀⠈⠗⠀⠀⠀⠀⠘⣷⣀⢀⣀⣀⠀⡀⢀⣌⡧⠂⠀⡞⠛⡟⠀⠀⠀⡠⠜
 *⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠓⠈⠙⠙⠋⠉⠁⠀⠀⠀⠀⠀⠀⠀⡂⠠⠤⢶
 */
Snake *create_snake(unsigned int x, unsigned int y, Direction direction) {
	Snake *snake = malloc(sizeof(Snake));
	if (snake == NULL) return NULL;
	snake->head = malloc(sizeof(PointList));
	if (snake->head == NULL) {
		free(snake);
		return NULL;
	}
	snake->length = 1;
	snake->tail = snake->head;

	snake->head->point.x = x;
	snake->head->point.y = y; 

	snake->moving_direction = direction;

	return snake;
}

Map *create_map(unsigned int width, unsigned int height) {
	srand(time(NULL));

	Map *map = malloc(sizeof(Map));
	if (map == NULL) return NULL;

	map->width = width;
	map->height = height;
	
	unsigned int snake_x = rand() % width;
	unsigned int snake_y = rand() % height;

	Direction snake_dir;
	if (rand() % 2 == 0) {
		if (snake_x > width / 2) {
			snake_dir = WEST;
		} else {
			snake_dir = EAST;
		}
	} else {
		if (snake_y > height / 2) {
			snake_dir = NORTH;
		} else {
			snake_dir = SOUTH;
		}
	}

	Snake *snake = create_snake(snake_x, snake_y, snake_dir);
	if (snake == NULL) {
		free_snake(snake);
		return NULL;
	}
	map->snake = snake;

	spawn_fruit(map);

	return map;
}

bool move_snake(Map *map) {
	PointList *neohead = malloc(sizeof(PointList));
	if (neohead == NULL) {
		exit(1);
	}
	neohead->point = map->snake->head->point;
	neohead->next = NULL;
	
	switch(map->snake->moving_direction) {
        case NORTH:
			neohead->point.y--;
			break;
        case WEST:
			neohead->point.x--;
			break;
        case EAST:
			neohead->point.x++;
			break;
        case SOUTH:
			neohead->point.y++;
			break;
          break;
    }

	if (point_out_of_bounds(neohead->point, map)) return false;

	if (position_in_snake(neohead->point, map->snake)) return false;

	map->snake->head->next = neohead;
	map->snake->head = neohead;

	if (neohead->point.x == map->fruit.x && neohead->point.y == map->fruit.y && !map->fruit_eaten) {
		map->fruit_eaten = true;
		map->snake->length++;
		spawn_fruit(map);
	} else {
		PointList *tmp = map->snake->tail;
		map->snake->tail = map->snake->tail->next;
		free(tmp);
	}

	return true;
}

void free_map(Map *map) {
	free_snake(map->snake);
	free(map);
}
