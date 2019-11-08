// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdbool.h>
#include <stdlib.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include "header/screen.h"
#include "header/game.h"
#include "header/data.h"
#include "header/pause.h"

extern void doTick(int);
void suspend();

int main() {
	// NCurses terminal initialization
	setlocale(LC_ALL, "");
	initscr();

	if (!(has_colors() && can_change_color())) {
		endwin();
		printf("Your console does not support color!");
		exit(EXIT_SUCCESS);
	}
	
	start_color();
	cbreak();
	noecho();
	timeout(deltaTime * 1000);
	curs_set(0);
	keypad(stdscr, true);
	
	// Load game data from storage
	if (loadData() != 0) {
		endwin();
		printf("Failed to load data. %s might be corrupted.", dataFileName);
		exit(EXIT_FAILURE);
	}

	// Start title screen
	setPromptMode(TITLE_PROMPT);
	drawScreen();
	initGameResolution();

	for (;;) {
		int key = getch();

		if (is_term_resized(row, column)) {
			clearScreen();
			drawScreen();
		}
		else {
			updateScreen(key);
			curs_set(0);

			if (!pause) {
				doTick(key);
			}
		}
	}
	
	endwin();
	return 0;
}

void suspend() {
	endwin();
	exit(EXIT_SUCCESS);
}