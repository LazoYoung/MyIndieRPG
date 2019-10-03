#include <stdio.h>
#include <ncursesw/curses.h>
#include <menu.h>
#include "screen.h"

extern int column, row, prompt_width, prompt_height;

static MENU *menu = NULL;
static WINDOW *menu_winow = NULL;

void drawScreen(short screen) {
	resize_term(row, column);
    border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    refresh();

    switch (screen) {
        case TITLE_SCREEN:
            drawTitleScreen();
            break;
    }
}

void drawPrompt(Prompt p) {
    int action;
    
    menu = new_menu(p.items);
    menu_winow = newwin(p.height, p.width, 0, 0);
    keypad(menu_winow, TRUE);
    set_menu_win(menu, menu_winow);
    set_menu_sub(menu, derwin(menu_winow, 3, 30, 3, 0));
    set_menu_mark(menu, " * ");
    wborder(menu_winow, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    refresh();

    post_menu(menu);
    wrefresh(menu_winow);

    while (1) {
        action = getch();

        if (action == KEY_DOWN) {
            menu_driver(menu, REQ_DOWN_ITEM);
        }
        else if (action == KEY_UP) {
            menu_driver(menu, REQ_UP_ITEM);
        }
        else if (action == 10) {
            // 메뉴 동작 함수를 받아온다.
            ITEM* item = current_item(menu);
            void (* func) = item_userptr(item);
            func;
            pos_menu_cursor(menu);
            break;
        }

        wrefresh(menu_winow);
    }

    deletePrompt();
}

void deletePrompt() {
    // TODO Free items in the menu    
    unpost_menu(menu);
    free_menu(menu);
    delwin(menu_winow);
}