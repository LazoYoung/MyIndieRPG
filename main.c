#define _XOPEN_SOURCE_EXTENDED
#define _GNU_SOURCE

#include <unistd.h>
#include <signal.h>
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include "screen.h"

short screen_mode = TITLE_SCREEN;
short prompt_mode = PROMPT_NONE;
int column = 130;
int row = 50;
static bool cont = TRUE;

int main() {
	// NCurses 터미널 스크린 초기화
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	
	// 타이틀 화면을 시작
	togglePrompt(TITLE_PROMPT);
	drawScreen();

	int result;
	struct timespec t;
	struct pollfd fds;
	t.tv_sec = 0;
	t.tv_nsec = 100;
	fds.fd = STDIN_FILENO;
	fds.events = POLLIN | POLLRDNORM;

	// 최대 100ms 동안 stdin 의 입력이 오기를 기다린다.
	// result = -1: 오류 / 0: 정상적 입력 없음 / 1: 정상적 입력
	while (cont) {
		int result = ppoll(&fds, 1, &t, NULL);
		int action = ERR;

		if (result > 0) {
			action = getch();
		}
		else if (result < 0) {
			continue;
		}

		if (prompt_mode != PROMPT_NONE) {
			refreshPrompt(action);
		}

		if (is_term_resized(row, column)) {
			clear();
			drawScreen();
		}
	}
	
	endwin();
	return 0;
}

void suspend() {
	cont = FALSE;
}