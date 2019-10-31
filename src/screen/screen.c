// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdbool.h>
#include <string.h>
#include <ncursesw/curses.h>
#include <ncursesw/menu.h>
#include <ncursesw/panel.h>
#include "header/screen.h"

ScreenMode screen_mode = TITLE_SCREEN;
PromptMode prompt_mode = PROMPT_NONE;
int column = 230;
int row = 70;
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
        case INVENTORY_SCREEN:
            drawInventoryScreen();
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
    menu_opts_off(menu, O_SHOWDESC);
    box(prompt_win[0], 0, 0);
    post_menu(menu);
    refresh();
    wrefresh(prompt_win[0]);
    wrefresh(prompt_win[1]);
}

/* 프롬프트 화면을 끈다. */
void deletePrompt() {
    if (menu == NULL) {
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
    prompt_mode = PROMPT_NONE;
}

MENU* getPromptMenu() {
    return menu;
}

/* 프롬프트 메뉴를 설정/전환한다. */
void setPromptMode(PromptMode mode) {
    deletePrompt();
    prompt_mode = mode;
    
    switch (prompt_mode) {
        case TITLE_PROMPT:
            prompt = getTitlePrompt();
            break;
        case TITLE_CHARACTER_PROMPT:
            prompt = getCharPrompt();
            break;
        case INV_CATEGORY_PROMPT:
            prompt = getCategoryPrompt();
            break;
        case INVENTORY_PROMPT:
            prompt = getInventoryPrompt();
            break;
    }

    drawPrompt();
}

void setScreenMode(ScreenMode mode) {
    screen_mode = mode;
    deletePrompt();
    clearScreen();
    drawScreen();
}

void setMenuOptions(Menu_Options options, bool on) {
    if (menu == NULL)
        return;

    if (on) {
        menu_opts_on(menu, options);
    } else {
        menu_opts_off(menu, options);
    }
}

/* 메뉴 입력을 받아 동작시키고, 프롬프트 화면을 갱신한다. */
static void refreshPrompt(int key) {
    if (menu == NULL || prompt_win[0] == NULL || prompt_win[1] == NULL) {
        return;
    }

    ITEM* item = current_item(menu);
    void (* buttonFunc)(ItemEvent, ITEM*) = (void(*)(ItemEvent, ITEM*)) item_userptr(item);
    int id = item_index(item);

    // TODO clear the smudge text
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
    else if (((int) item_opts(item) & O_SELECTABLE) == O_SELECTABLE) {
        switch (key) {
            case 10:
                (* buttonFunc)(CLICK, item);
                refresh();
                return;
            case ' ':
                touchwin(prompt_win[0]);
                (* buttonFunc)(SELECT, item);
                wrefresh(prompt_win[0]);
                wrefresh(prompt_win[1]);
                refresh();
                return;
        }
    }

    touchwin(prompt_win[0]);
    (* buttonFunc)(HOVER, item);
    wrefresh(prompt_win[0]);
    wrefresh(prompt_win[1]);
    refresh();
}