// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include "screen.h"
#include "game.h"

static bool cont = true;

void suspend();

int main() {
	// NCurses 터미널 스크린 초기화
	setlocale(LC_ALL, "");
	initscr();

	if (!has_colors()) {
		endwin();
		printf("Your console does not support color!");
		exit(EXIT_SUCCESS);
	}
	
	cbreak();
	noecho();
	timeout(deltaTime * 1000);
	curs_set(0);
	keypad(stdscr, true);
	
	// 타이틀 화면을 시작
	togglePrompt(TITLE_PROMPT);
	drawScreen();
	initGameResolution();

	while (cont) {
		int key = getch();

		if (is_term_resized(row, column)) {
			clearScreen();
			drawScreen();
		}
		else {
			refreshScreen(key);

			if (!doTick(key)) {
				suspend();
			}
		}
	}
	
	endwin();
	return 0;
}

void suspend() {
	cont = false;
}