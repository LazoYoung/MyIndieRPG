// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <string.h>
#include <ncursesw/curses.h>
#include <ncursesw/menu.h>
#include <ncursesw/panel.h>
#include "header/screen.h"

char screen_mode = TITLE_SCREEN;
char prompt_mode = PROMPT_NONE;
int column = 130;
int row = 40;
Prompt prompt = {0, 0, 0, 0, 0, NULL};
static MENU *menu = NULL;
static WINDOW *prompt_win[2] = { NULL }; // 0: 테두리, 1: 버튼

static void refreshPrompt(int);

/* 화면을 생성한다. */
void drawScreen() {
	resizeterm(row, column);
    clear();
    border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    refresh();

    switch (screen_mode) {
        case TITLE_SCREEN:
            drawTitleScreen();
            break;
        case GAME_SCREEN:
            initGameScreen();
            break;
    }
}

/* 화면을 갱신한다. */
void refreshScreen(int key) {
    if (prompt_mode != PROMPT_NONE) {
        refreshPrompt(key);
    }
    if (screen_mode == GAME_SCREEN) {
        drawGameScreen();
    }
}

void clearScreen() {
    if (screen_mode == GAME_SCREEN) {
        clearGameScreen();
    }
}

/* 프롬프트 화면을 그린다. */
void drawPrompt() {
    menu = new_menu(prompt.items);
    prompt_win[0] = newwin(prompt.height, prompt.width, prompt.y, prompt.x);
    prompt_win[1] = derwin(prompt_win[0], item_count(menu), 20, prompt.desc_lines + 3, 3);

    keypad(prompt_win[0], TRUE);
    set_menu_win(menu, prompt_win[0]);
    set_menu_sub(menu, prompt_win[1]);
    set_menu_mark(menu, "*");
    box(prompt_win[0], 0, 0);
    post_menu(menu);
    refresh();
    wrefresh(prompt_win[0]);
    wrefresh(prompt_win[1]);
}

/* 프롬프트 화면을 끈다. */
void deletePrompt() {
    if (menu == NULL || prompt_mode == PROMPT_NONE) {
        return;
    }

    ITEM** items = menu_items(menu);
    int count = item_count(menu);
    
    wclear(prompt_win[0]);
    touchwin(prompt_win[0]);
    wrefresh(prompt_win[0]);
    wrefresh(prompt_win[1]);
    // unpost_menu() must precede delwin() to make sure that they're unlinked
    unpost_menu(menu);
    free_menu(menu); 
    delwin(prompt_win[0]);
    delwin(prompt_win[1]);

    for (int i = 0; i < count; i++) {
        free_item(items[i]);
    }

    items = NULL;
    menu = NULL;

    togglePrompt(PROMPT_NONE);
}

/* 프롬프트 메뉴 mode를 전환한다. 매크로는 screen.h에 정의되어 있음 */
void togglePrompt(char mode) {
    prompt_mode = mode;
    deletePrompt();
    drawScreen();
}

void toggleScreen(char mode) {
    screen_mode = mode;
    deletePrompt();
    clearScreen();
    drawScreen();
}

WINDOW* getPromptWindow() {
    return prompt_win[0];
}

/* 메뉴 입력을 받아 동작시키고, 프롬프트 화면을 갱신한다. */
static void refreshPrompt(int key) {
    if (menu == NULL || prompt_win[0] == NULL || prompt_win[1] == NULL) {
        return;
    }

    ITEM* item = current_item(menu);
    void (* buttonFunc)(int) = (void(*)(int)) item_userptr(item);
    int id = item_index(item);

    if (key == KEY_DOWN && id < item_count(menu) - 1) {
        werase(prompt_win[0]);
        box(prompt_win[0], 0, 0);
        menu_driver(menu, REQ_DOWN_ITEM);
        refresh();
    }
    else if (key == KEY_UP && id > 0) {
        werase(prompt_win[0]);
        box(prompt_win[0], 0, 0);
        menu_driver(menu, REQ_UP_ITEM);
        refresh();
    }
    else if (key == 10) {
        deletePrompt();
        buttonFunc(1);
        refresh();
        return;
    }
    else {
        touchwin(prompt_win[0]);
        buttonFunc(0);
        wrefresh(prompt_win[0]);
        wrefresh(prompt_win[1]);
        refresh();
    }
}