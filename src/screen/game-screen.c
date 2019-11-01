// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <ncursesw/curses.h>
#include <ncursesw/panel.h>
#include "header/screen.h"
#include "header/game.h"
#include "header/level.h"

enum Axis { X, Y };

static PANEL* panels[WORLD_WIN + 1];
static int row_, column_, init_y, init_x, ctr_y, ctr_x;

static void drawStatus(WINDOW*);
static void drawGuage(WINDOW*, const char*, Color, Color, char);
static void drawEquipment(WINDOW*, ItemCategory);
static void drawControlAid(WINDOW*);
static void drawTiles();
static void drawEntities();
static bool getTileCoordByScreen(enum Axis, int, int*);
static bool getScreenCoordByTile(enum Axis, int, int*);

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
    WINDOW *mp_win_0 = newwin(3, 40, 3, 0);
    WINDOW *mp_win_1 = derwin(mp_win_0, 1, 38, 1, 1);
    WINDOW *stat_win = newwin(5, 25, init_y + row_, init_x + 1);
    WINDOW *weapon_win = newwin(5, 20, init_y + row_, init_x + 27);
    WINDOW *armory_win = newwin(5, 20, init_y + row_, init_x + 48);
    WINDOW *potion_win = newwin(5, 20, init_y + row_, init_x + 69);
    WINDOW *ctrl_win = newwin(5, 30, init_y + row_, column_ - 30);
    panels[WORLD_WIN] = new_panel(world_win);
    panels[HEALTH_WIN] = new_panel(hp_win_0);
    panels[MANA_WIN] = new_panel(mp_win_0);
    panels[HEALTH_G_WIN] = new_panel(hp_win_1);
    panels[MANA_G_WIN] = new_panel(mp_win_1);
    panels[STAT_WIN] = new_panel(stat_win);
    panels[WEAPON_WIN] = new_panel(weapon_win);
    panels[ARMORY_WIN] = new_panel(armory_win);
    panels[POTION_WIN] = new_panel(potion_win);
    panels[CONTROL_WIN] = new_panel(ctrl_win);
    
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
}

void drawGameScreen() {
    WINDOW* health_bg = getGameWindow(HEALTH_WIN);
    WINDOW* health_w = getGameWindow(HEALTH_G_WIN);
    WINDOW* mana_bg = getGameWindow(MANA_WIN);
    WINDOW* mana_w = getGameWindow(MANA_G_WIN);
    WINDOW* stat_w = getGameWindow(STAT_WIN);
    WINDOW* weapon_w = getGameWindow(WEAPON_WIN);
    WINDOW* armory_w = getGameWindow(ARMORY_WIN);
    WINDOW* potion_w = getGameWindow(POTION_WIN);
    WINDOW* ctrl_w = getGameWindow(CONTROL_WIN);
    Entity* player = getEntityByID(0);

    for (int i = 0; i <= WORLD_WIN; i++) {
        WINDOW *w = getGameWindow(i);

        if (w != NULL)
            werase(w);
    }

    box(health_bg, ACS_VLINE, ACS_HLINE);
    box(mana_bg, ACS_VLINE, ACS_HLINE);
    box(stat_w, ACS_VLINE, ACS_HLINE);
    box(weapon_w, ACS_VLINE, ACS_HLINE);
    box(armory_w, ACS_VLINE, ACS_HLINE);
    box(potion_w, ACS_VLINE, ACS_HLINE);
    box(ctrl_w, ACS_VLINE, ACS_HLINE);

    if (inGame && player != NULL) {
        drawGuage(health_w, "Health", GREEN, RED, player->health);
        drawGuage(mana_w, "Mana", CYAN, BLACK, player->mp);
        drawStatus(stat_w);
        drawEquipment(weapon_w, WEAPON);
        drawEquipment(armory_w, ARMORY);
        drawEquipment(potion_w, POTION);
        drawControlAid(ctrl_w);
        drawTiles();
        drawEntities();
    }

    refresh();
    update_panels();
    doupdate();
}

void clearGameScreen() {
    WINDOW* w = NULL;

    for (int i=0; i<=WORLD_WIN; i++) {
        w = getGameWindow(i);
        wclear(w);
        del_panel(panels[i]);
        delwin(w);
    }
}

WINDOW* getGameWindow(WindowType type) {
    if (panels[type] == NULL)
        return NULL;

    return panel_window(panels[type]);
}

static void drawStatus(WINDOW* win) {
    int lvl = p_attr.level;

    mvwprintw(win, 1, 2, "Character: %s", p_attr.name);
    mvwprintw(win, 2, 2, "Level: %d (Exp. %d/%d)", lvl, p_attr.exp, getExpCap(lvl));
    mvwprintw(win, 3, 2, "Coin: %d", inv.coin);
}

static void drawGuage(WINDOW* win, const char* alias, Color color_fg, Color color_bg, char value) {
    char value_ = value / 4;

    waddch(win, ' ');
    wattron(win, A_BOLD);
    waddstr(win, alias);
    wattroff(win, A_BOLD);
    waddch(win, ' ');

    wattron(win, COLOR_PAIR(color_fg));
    mvwaddnstr(win, 0, 13, "                         ", value_);
    wattroff(win, COLOR_PAIR(color_fg));

    wattron(win, COLOR_PAIR(color_bg));
    mvwaddstr(win, 0, 13 + value_, "                         ");
    wattroff(win, COLOR_PAIR(color_bg));
}

static void drawEquipment(WINDOW* win, ItemCategory category) {
    GItem *item = inv.equipment[category];
    const char *name = NULL;
    const char *title = NULL;

    if (item != NULL) {
        name = getItemName(item->type);
    }

    switch (category) {
        case WEAPON:
            title = "Weapon";
            break;
        case ARMORY:
            title = "Armory";
            break;
        case POTION:
            title = "Potion";
            break;
    }

    mvwprintw(win, 1, 2, title);
    mvwprintw(win, 3, 2, "%s", name != NULL ? name : "Not equipped.");
}

static void drawControlAid(WINDOW* win) {
    mvwprintw(win, 1, 9, "Control Keys");
    mvwprintw(win, 2, 2, "Move: WASD");
    mvwprintw(win, 2, 15, "Attack: K");
    mvwprintw(win, 3, 2, "Potion: H/M");
    mvwprintw(win, 3, 15, "Inventory: I");
}

static void drawTiles() {
    Entity *entity = getEntity(p_attr.name);
    WINDOW *win = getGameWindow(WORLD_WIN);
    Location loc;

    if (entity) {
        loc = entity->loc;
    }

    werase(win);
    box(win, ACS_VLINE, ACS_HLINE);
    
    // Convert coordinates from screen to tile
    for (int y_ = 0; y_ < row_; y_++) {
        Tile prev_t = AIR;
        int tile_y;

        if (getTileCoordByScreen(Y, y_, &tile_y) == false)
            continue;

        for (int x_ = 0; x_ < column_; x_++) {
            int tile_x;
            Tile tile;

            if (getTileCoordByScreen(X, x_, &tile_x) == false)
                continue;
            
            switch (tile = getTileAt(tile_x, tile_y)) {
                case BLOCK:
                    wattron(win, COLOR_PAIR(WHITE));
                    mvwaddch(win, y_, x_, 'B');
                    wattroff(win, COLOR_PAIR(WHITE));
                    break;
                case TRAIL:
                    wattron(win, COLOR_PAIR(CYAN));
                    mvwaddch(win, y_, x_, 'T');
                    wattroff(win, COLOR_PAIR(CYAN));
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

                        if (prev_t != tile) {
                            char label[20];
                            const char* name = getStageName(portal->dest);

                            snprintf(label, 20, "-> %s", name);
                            prev_t = tile;
                            mvwprintw(win, y_ - 2, x_ - 4, label);
                        }
                    }
                    break;
                }
            }
        }
    }
}

static void drawEntities() {
    int id = 0, row = 0;
    Entity* iter = getEntityByID(id);
    
    while (iter != NULL) {
        Location loc = iter->loc;
        Texture skin = iter->skin;
        WINDOW* win = getGameWindow(WORLD_WIN);
        int color = COLOR_PAIR(skin.color);
        int x, y;

        wattron(win, color);

        for (y = 0; y < 9; y++) {
            for (x = 0; x < 9; x++) {
                if (!skin.map[y][x])
                    continue;

                if (id == 0) { // is Player
                    mvwaddch(win, ctr_y + y - 4, ctr_x + x - 4, ' ');
                }
                else { // is Mob
                    int scr_x, scr_y;
                    int y_, x_;

                    if (!getScreenCoordByTile(X, loc.pos[0], &scr_x)
                        || !getScreenCoordByTile(Y, loc.pos[1], &scr_y))
                            continue;

                    y_ = scr_y + y - 4;
                    x_ = scr_x + x - 4;

                    if (x_ < column_ && y_ < row_)
                        mvwaddch(win, y_, x_, ' ');
                }
            }
        }
        
        wattroff(win, color);

        // TODO Debug
        mvwprintw(win, 10 + row++, 2, "%s's position: %.2f %.2f", iter->name, loc.pos[0], loc.pos[1]);
        mvwprintw(win, 10 + row++, 2, "%s's velocity: %.2f %.2f", iter->name, loc.spd[0], loc.spd[1]);
        mvwprintw(win, 10 + row++, 2, "%s's health: %.1f", iter->name, iter->health);

        iter = getEntityByID(++id);
    }
}

/**
 * Converts a Screen coordinate into Tile coordinate.
 * Pass the coordinate (in either X or Y depending on Axis A) into INPUT.
 * Then, RESULT will point to the computed tile coordinate.
 **/
static bool getTileCoordByScreen(enum Axis a, int input, int *result) {
    Entity *player = getEntityByID(0);
    int x, y;

    if (player == NULL)
        return false;

    if (a == X) {
        x = player->loc.pos[0] + input - ctr_x;

        if (x < level_width && x >= 0) {
            *result = x;
            return true;
        }
    }
    else { // a == Y
        y = player->loc.pos[1] + ctr_y - input;

        if (y < level_height && y >= 0) {
            *result = y;
            return true;
        }
    }
    
    return false;
}

/**x = tile_x + ctr_x - p_x; y = ctr_y - tile_y + p_y;
 * Converts a Tile coordinate into Screen coordinate.
 * Pass the coordinate (in either X or Y depending on Axis A) into INPUT.
 * Then, RESULT will point to the computed screen coordinate.
 **/
static bool getScreenCoordByTile(enum Axis a, int input, int *result) {
    Entity *player = getEntityByID(0);
    int x, y;

    if (player == NULL)
        return false;

    if (a == X) {
        x = input + ctr_x - player->loc.pos[0];

        if (x < level_width && x >= 0) {
            *result = x;
            return true;
        }
    }
    else {
        y = ctr_y - input + player->loc.pos[1];

        if (y < level_height && y >= 0) {
            *result = y;
            return true;
        }
    }
    
    return false;
}