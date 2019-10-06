#include <stdio.h>
#include <stdlib.h>
#include <menu.h>
#include <panel.h>
#include <ncursesw/curses.h>
#include "screen.h"

extern int column, row;

extern void suspend();
static Prompt getTitlePrompt();
static Prompt getCharPrompt();
static void startGame();
static void onStart(int);
static void onQuit(int);
static void onKirito(int);

void drawTitleScreen() {
    mvaddch(10, column / 2 - 10, ACS_DIAMOND);
    printw(" Sword Art Online ");
    addch(ACS_DIAMOND);
    mvprintw(12, column / 2 - 10, "An MMO-RPG Prototype");
    mvprintw(13, column / 2 - 18, "2019136063 parkcymil@koreatech.ac.kr");

    switch (prompt_mode) {
        case TITLE_PROMPT:
            prompt = getTitlePrompt();
            break;
        case TITLE_CHARACTER_PROMPT:
            prompt = getCharPrompt();
            break;
        default:
            return;
    }
    
    drawPrompt();
}

/* 시작 버튼을 누른 이벤트 */
static void onStart(int click) {
    if (click) {
        togglePrompt(TITLE_CHARACTER_PROMPT);
        return;
    }

    mvwprintw(getPromptWindow(), 1, 3, "Hit Enter to select your character!");
}

/* 종료 버튼을 누른 이벤트 */
static void onQuit(int click) {
    if (click) {
        suspend();
        return;
    }
    
    mvwprintw(getPromptWindow(), 1, 3, "Hit Enter to exit the game.");
}

static void onKirito(int click) {
    if (click) {
        startGame();
        return;
    }

    mvwprintw(getPromptWindow(), 1, 3, "Hit Enter to select %s", "Kirito");
}

static void onReturn(int click) {
    if (!click) {
        return;
    }

    togglePrompt(TITLE_PROMPT);
}

static void startGame() {
    // TODO Implement
}

static Prompt getTitlePrompt() {
    ITEM **myItems = (ITEM**) calloc(3, sizeof(ITEM*));
    myItems[0] = new_item("◎", "Start Game");
    myItems[1] = new_item("◎", "Quit");
    myItems[2] = NULL; // 말단 원소는 NULL값으로 지정해야 함

    // 버튼 동작 함수를 NCurse로 넘겨준다.
    set_item_userptr(myItems[0], (void*) onStart);
    set_item_userptr(myItems[1], (void*) onQuit);

    Prompt p = {40, 15, column / 2 - 20, row / 2 - 5, 1, myItems};
    return p;
}

static Prompt getCharPrompt() {
    ITEM **myItems = (ITEM**) calloc(6, sizeof(ITEM*));
    myItems[0] = new_item("▣", "Kirito");
    myItems[1] = new_item("▣", "Asuna");
    myItems[2] = new_item("▣", "Klein");
    myItems[3] = new_item("▣", "Agil");
    myItems[4] = new_item("←", "Go back");
    myItems[5] = NULL;

    // 버튼 동작 함수를 NCurse로 넘겨준다.
    set_item_userptr(myItems[0], (void*) onKirito);
    set_item_userptr(myItems[1], (void*) onKirito);
    set_item_userptr(myItems[2], (void*) onKirito);
    set_item_userptr(myItems[3], (void*) onKirito);
    set_item_userptr(myItems[4], (void*) onReturn);

    Prompt p = {40, 20, column / 2 - 20, row / 2 - 10, 8, myItems};
    return p;
}