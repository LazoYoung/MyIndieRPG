#undef _GNU_SOURCE
#define _GNU_SOURCE

#include <ncursesw/curses.h>
#include <ncursesw/panel.h>
#include "screen.h"
#include "game.h"

static WINDOW *hp_win[2];
static PANEL *hp_panel[2];

void drawGameScreen() {
    hp_win[0] = newwin(3, 40, 5, 5);
    hp_win[1] = derwin(hp_win[0], 1, 38, 1, 1);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_GREEN);

    waddch(hp_win[1] , ' ');
    wattron(hp_win[1], A_BOLD);
    waddstr(hp_win[1], p_attr.name);
    wattroff(hp_win[1], A_BOLD);
    waddch(hp_win[1] , ' ');
    wattron(hp_win[1], COLOR_PAIR(1));
    mvwaddwstr(hp_win[1], 0, 13, L"                         "); // 25 spaces
    wattroff(hp_win[1], COLOR_PAIR(1));
    box(hp_win[0], 0, 0);
    hp_panel[0] = new_panel(hp_win[0]);
    hp_panel[1] = new_panel(hp_win[1]);
    update_panels();
    doupdate();
}