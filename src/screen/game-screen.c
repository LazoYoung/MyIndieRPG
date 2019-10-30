// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <ncursesw/curses.h>
#include <ncursesw/panel.h>
#include "header/screen.h"
#include "header/game.h"
#include "header/level.h"

static PANEL* panels[3];
static int row_, column_, init_y, init_x, ctr_y, ctr_x;

static void drawTiles();
static void drawEntities();

void initGameResolution() {
    row_ = row - 10;
    column_ = column - 2;
    init_y = 0;
    init_x = 1;
    ctr_y = row_ / 2 + init_y;
    ctr_x = column_ / 2 + init_x;
}

void initGameScreen() {
    WINDOW *world_win = newwin(row_, column_, init_y, init_x);
    WINDOW *hp_win_0 = newwin(3, 40, 0, 0);
    WINDOW *hp_win_1 = derwin(hp_win_0, 1, 38, 1, 1);
    panels[0] = new_panel(world_win);
    panels[1] = new_panel(hp_win_0);
    panels[2] = new_panel(hp_win_1);
    
    // Initialize basic screen colors
    init_pair(1, COLOR_RED, COLOR_RED);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_BLUE, COLOR_BLUE);
    init_pair(4, COLOR_WHITE, COLOR_WHITE);
    init_pair(5, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(7, COLOR_BLACK, COLOR_BLACK);
    init_pair(8, COLOR_CYAN, COLOR_CYAN);

    timeout(deltaTime * 1000);
	keypad(world_win, true);

    update_panels();
    doupdate();

    generateLevel();
}

void drawGameScreen() {
    int health = p_attr.health / 4;
    WINDOW* guage_bg = panel_window(panels[1]);
    WINDOW* guage = panel_window(panels[2]);

    werase(guage_bg);
    werase(guage);
    box(guage_bg, ACS_VLINE, ACS_HLINE);

    waddch(guage, ' ');
    wattron(guage, A_BOLD);
    waddstr(guage, p_attr.name);
    wattroff(guage, A_BOLD);
    waddch(guage, ' ');

    wattron(guage, COLOR_PAIR(GREEN));
    mvwaddnstr(guage, 0, 13, "                         ", health);
    wattroff(guage, COLOR_PAIR(GREEN));

    wattron(guage, COLOR_PAIR(RED));
    mvwaddstr(guage, 0, 13 + health, "                         ");
    wattroff(guage, COLOR_PAIR(RED));
    
    if (inGame) {
        drawTiles();
        drawEntities();
    }

    refresh();
    update_panels();
    doupdate();
}

void clearGameScreen() {
    WINDOW* w0 = panel_window(panels[0]);
    WINDOW* w1 = panel_window(panels[1]);
    WINDOW* w2 = panel_window(panels[2]);
    wclear(w0);
    wclear(w1);
    touchwin(w1);
    del_panel(panels[0]);
    del_panel(panels[1]);
    del_panel(panels[2]);
    delwin(w0);
    delwin(w1);
    delwin(w2);
    
    destructLevel();
}

static void drawTiles() {
    Entity entity = *getEntity(p_attr.name);
    WINDOW *win = panel_window(panels[0]);
    Location loc;

    if (entity.valid) {
        loc = entity.loc;
    }

    werase(win);
    box(win, ACS_VLINE, ACS_HLINE);
    
    // Convert coordinates from screen to tile
    for (int y_ = 0; y_ < row_; y_++) {
        int y = loc.pos[1] + ctr_y - y_;

        if (y < 0 || y >= level_height)
            continue;

        for (int x_ = 0; x_ < column_; x_++) {
            int x = loc.pos[0] + x_ - ctr_x;

            if (x < 0 || x >= level_width) {
                continue;
            }

            Tile tile = getTileAt(x, y);

            switch (tile) {
                case BLOCK:
                    wattron(win, COLOR_PAIR(WHITE));
                    mvwaddch(win, y_, x_, 'B');
                    wattroff(win, COLOR_PAIR(WHITE));
                    break;
                case PORTAL_1:
                case PORTAL_2:
                case PORTAL_3:
                case PORTAL_4:
                case PORTAL_5: {
                    Portal *portal = getPortal(tile);

                    if (portal != NULL) {
                        wattron(win, COLOR_PAIR(portal->color));
                        mvwaddch(win, y_, x_, 'P');
                        wattroff(win, COLOR_PAIR(portal->color));
                    }
                    break;
                }
            }
        }
    }
}

static void drawEntities() {
    Entity* iter = getEntity(NULL);
    int cnt = 0;
    
    while (iter && iter->valid) {
        Location loc = iter->loc;
        Texture skin = iter->skin;
        int color = COLOR_PAIR(10);
        WINDOW* win = panel_window(panels[0]);
        int x, y;

        init_pair(10, COLOR_WHITE, skin.color);
        wattron(win, color);
        for (y = 0; y < 9; y++) {
            for (x = 0; x < 9; x++) {
                if (skin.map[y][x]) {
                    mvwaddch(win, ctr_y + y - 4, ctr_x + x - 4, ' ');
                }
            }
        }
        wattroff(win, color);

        // TODO Debug
        mvwprintw(panel_window(panels[0]), 3 + cnt++, 0, "%s's position: %.2f %.2f", iter->name, iter->loc.pos[0], iter->loc.pos[1]);
        mvwprintw(panel_window(panels[0]), 3 + cnt++, 0, "%s's speed: %.2f %.2f", iter->name, iter->loc.spd[0], iter->loc.spd[1]);

        iter = iter->next;
    }
}