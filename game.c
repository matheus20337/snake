#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>
#include "snake.h"

#define DEFAULT_WIDTH 30
#define DEFAULT_HEIGHT 25

/* draws the game map. Excepts a window large enough to print the map */
static void draw(WINDOW *display, Map *map) {
	werase(display);

	box(display, 0, 0);

	if (!map->fruit_eaten) {
		mvwaddch(display, map->fruit.y, map->fruit.x, '*');
	}

	for (PointList *i = map->snake->tail; i != NULL; i = i->next) {
		mvwaddch(display, i->point.y, i->point.x, '#');
	}

	wrefresh(display);
}

int main(int argc, char **argv) {
	setlocale(LC_ALL, "");

	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, true);
	curs_set(0);

	const unsigned short target_fps = 5;
	bool quit = false;

	Map *map;
	if (argc == 3) {
		unsigned int width = (unsigned int)strtol(argv[1], NULL, 10);
		unsigned int height = (unsigned int)strtol(argv[2], NULL, 10);
		map = create_map(width, height);
	} else {
		map = create_map(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	}
	
	if (map == NULL) {
		fprintf(stderr, "ERROR: Failed to allocate map");
		endwin();
		exit(1);
	}

	WINDOW *map_window = newwin(map->height, map->width, (LINES/2) - (map->height/2), (COLS/2) - (map->width/2));

	draw(map_window, map);

	while (!quit) {
		int key;

		/* Get Input */
		key = getch();

		switch(key) {
			case 'w':
				map->snake->moving_direction = NORTH;
				break;
			case 'a':
				map->snake->moving_direction = WEST;
				break;
			case 's':
				map->snake->moving_direction = SOUTH;
				break;
			case 'd':
				map->snake->moving_direction = EAST;
				break;
			case 'q':
				quit = true;
				break;
		}

		/* Process game logic */
		if (!move_snake(map)) {
			nodelay(stdscr, false);
			/* TODO: Proper game over logic */
			clear();
			mvprintw(0, 0, "Game Over");
			getch();
			quit = true;
		}

		/* Draw frame */
		if (!quit) draw(map_window, map);

		/* TODO: calculate a proper wait time */
		napms((1.0f/(float)target_fps) * 1000);
	}

	free_map(map);

	delwin(map_window);
	endwin();
	return 0;
}
