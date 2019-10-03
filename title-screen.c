#include <stdio.h>
#include <stdlib.h>
#include <menu.h>
#include <ncursesw/curses.h>
#include "screen.h"

#define ITEM_CNT 3

extern int column, row;

extern void terminate();
static Prompt getPrompt();

void drawTitleScreen() {
    Prompt prompt = getPrompt();
    mvaddch(10, column / 2 - 12, ACS_DIAMOND);
    printw(" Sword Art Online ");
    addch(ACS_DIAMOND);
    mvprintw(12, column / 2 - 10, "An MMO-RPG Prototype");
    mvprintw(13, column / 2 - 18, "2019136063 parkcymil@koreatech.ac.kr");
    drawPrompt(prompt);
}

// 시작 버튼을 누른 이벤트
static void onStart() {
    mvprintw(0, 0, "Starting game...");
}

// 종료 버튼을 누른 이벤트
static void onQuit() {
    deletePrompt();
}

static Prompt getPrompt() {
    ITEM **myItems = (ITEM**) calloc(ITEM_CNT + 1, sizeof(ITEM*));
    myItems[0] = new_item("start", "Start Game");
    myItems[1] = new_item("quit", "Quit");
    myItems[2] = (ITEM*) NULL;

    // 버튼 동작 함수를 NCurse로 넘겨준다.
    set_item_userptr(myItems[0], (void*) onStart);
    set_item_userptr(myItems[1], (void*) onQuit);

    Prompt p = {50, 10, myItems};
    return p;
}