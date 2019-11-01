// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdlib.h>
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
static PANEL *prompt_pan[2] = { NULL }; // 0: 테두리, 1: 버튼

static void refreshPrompt(int);
static Prompt getDialoguePrompt();
static void onDialogueConfirm(ItemEvent, ITEM*);

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
    WINDOW *win0, *win1;

    menu = new_menu(prompt.items);
    win0 = newwin(prompt.height, prompt.width, prompt.y, prompt.x);
    win1 = derwin(win0, item_count(menu), 20, prompt.desc_lines + 3, 3);
    prompt_pan[0] = new_panel(win0);
    prompt_pan[1] = new_panel(win1);

    keypad(win0, TRUE);
    curs_set(0);
    set_menu_win(menu, win0);
    set_menu_sub(menu, win1);
    set_menu_mark(menu, "*");
    menu_opts_off(menu, O_SHOWDESC);
    box(win0, 0, 0);
    post_menu(menu);
    refresh();
    wrefresh(win0);
    wrefresh(win1);
    update_panels();
}

/* 프롬프트 화면을 끈다. */
void deletePrompt() {
    if (menu == NULL) {
        return;
    }

    ITEM** items = menu_items(menu);
    WINDOW* win[2] = {getPromptWindow(0), getPromptWindow(1)};
    int count = item_count(menu);
    
    wclear(win[0]);
    touchwin(win[0]);
    wrefresh(win[0]);
    wrefresh(win[1]);
    // unpost_menu() must precede delwin() to make sure that they're unlinked
    unpost_menu(menu);
    free_menu(menu);
    del_panel(prompt_pan[0]);
    del_panel(prompt_pan[1]);
    delwin(win[0]);
    delwin(win[1]);

    for (int i = 0; i < count; i++) {
        free_item(items[i]);
    }

    items = NULL;
    menu = NULL;
    prompt_pan[0] = NULL;
    prompt_pan[1] = NULL;
    prompt_mode = PROMPT_NONE;
}

MENU* getPromptMenu() {
    return menu;
}

/* 0: Background, 1: Button box */
WINDOW* getPromptWindow(int index) {
    PANEL* pan = prompt_pan[index];

    if (pan == NULL) return NULL;
    
    return panel_window(pan);
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
        case DIALOGUE_PROMPT:
            prompt = getDialoguePrompt();
            break;
        default:
            return;
    }

    drawPrompt();
}

void setScreenMode(ScreenMode mode) {
    deletePrompt();
    clearScreen();
    screen_mode = mode;
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
    if (menu == NULL || getPromptWindow(0) == NULL || getPromptWindow(1) == NULL) {
        return;
    }

    ITEM* item = current_item(menu);
    void (* buttonFunc)(ItemEvent, ITEM*) = (void(*)(ItemEvent, ITEM*)) item_userptr(item);
    int id = item_index(item);

    // TODO clear the smudge text
    if (key == KEY_DOWN && id < item_count(menu) - 1) {
        werase(getPromptWindow(0));
        box(getPromptWindow(0), 0, 0);
        menu_driver(menu, REQ_DOWN_ITEM);
        refresh();
    }
    else if (key == KEY_UP && id > 0) {
        werase(getPromptWindow(0));
        box(getPromptWindow(0), 0, 0);
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
                touchwin(getPromptWindow(0));
                (* buttonFunc)(SELECT, item);
                wrefresh(getPromptWindow(0));
                wrefresh(getPromptWindow(1));
                refresh();
                return;
        }
    }

    touchwin(getPromptWindow(0));
    (* buttonFunc)(HOVER, item);
    wrefresh(getPromptWindow(0));
    wrefresh(getPromptWindow(1));
    refresh();
}

static Prompt getDialoguePrompt() {
    Prompt p;
    ITEM** items;
    
    items = calloc(2, sizeof(ITEM*));
    items[0] = new_item("Confirm", "confirm");
    items[1] = NULL;
    set_item_userptr(items[0], onDialogueConfirm);

    p.desc_lines = 5;
    p.items = items;
    p.height = 15;
    p.width = 80;
    p.y = 10;
    p.x = column / 2 - 40;
    return p;
}

static void onDialogueConfirm(ItemEvent event, ITEM* item) {
    if (event == CLICK)
        deletePrompt();
}