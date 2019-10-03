#define _XOPEN_SOURCE_EXTENDED

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include "screen.h"

char screen = TITLE_SCREEN;
int column = 130, row = 50;
static int cont = 1;

extern void drawScreen(short);

int main() {
	// NCurses 터미널 스크린 초기화
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	
	drawScreen(screen);
	endwin();
	return 0;
}