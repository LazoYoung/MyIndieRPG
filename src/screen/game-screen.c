// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <math.h>
#include <string.h>
#include <ncursesw/curses.h>
#include <ncursesw/panel.h>
#include "header/data.h"
#include "header/screen.h"
#include "header/game.h"
#include "header/level.h"

enum Axis { X, Y };

static PANEL* panels[WORLD_WIN + 1]; // Screen panel cache (Not a game-data)
static int row_, column_, init_y, init_x, ctr_y, ctr_x;

static void drawStatus(WINDOW*);
static void drawGuage(WINDOW* win, Color color_fg, Color color_bg, int length, float ratio);
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
    WINDOW *stat_win = newwin(6, 25, init_y + row_, init_x + 1);
    WINDOW *weapon_win = newwin(6, 20, init_y + row_, init_x + 27);
    WINDOW *armory_win = newwin(6, 20, init_y + row_, init_x + 48);
    WINDOW *potion_win = newwin(6, 20, init_y + row_, init_x + 69);
    WINDOW *ctrl_win = newwin(6, 35, init_y + row_, column_ - 35);
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
    init_pair(RED, COLOR_RED, COLOR_RED);
    init_pair(GREEN, COLOR_GREEN, COLOR_GREEN);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLUE);
    init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(CYAN, COLOR_CYAN, COLOR_CYAN);

    timeout(deltaTime * 1000);
	keypad(world_win, true);

    update_panels();
    doupdate();
}

void drawGameScreen() {
    WINDOW* w[WT_SIZE];
    Entity* player = getEntityByID(0);

    for (int i = 0; i < WT_SIZE; i++) {
        w[i] = getGameWindow(i);

        if (w[i] != NULL) {
            werase(w[i]);
        }
    }

    box(w[HEALTH_WIN], ACS_VLINE, ACS_HLINE);
    box(w[MANA_WIN], ACS_VLINE, ACS_HLINE);
    box(w[STAT_WIN], ACS_VLINE, ACS_HLINE);
    box(w[WEAPON_WIN], ACS_VLINE, ACS_HLINE);
    box(w[ARMORY_WIN], ACS_VLINE, ACS_HLINE);
    box(w[POTION_WIN], ACS_VLINE, ACS_HLINE);
    box(w[CONTROL_WIN], ACS_VLINE, ACS_HLINE);

    if (inGame && player) {
        waddch(w[HEALTH_G_WIN], ' ');
        wattron(w[HEALTH_G_WIN], A_BOLD);
        waddstr(w[HEALTH_G_WIN], "Health");
        wattroff(w[HEALTH_G_WIN], A_BOLD);
        waddch(w[HEALTH_G_WIN], ' ');
        wmove(w[HEALTH_G_WIN], 0, 13);
        drawGuage(w[HEALTH_G_WIN], GREEN, RED, 25, (float) player->health / playerData[playerType][P_MAX_HEALTH]);
        waddch(w[MANA_G_WIN], ' ');
        wattron(w[MANA_G_WIN], A_BOLD);
        waddstr(w[MANA_G_WIN], "Mana");
        wattroff(w[MANA_G_WIN], A_BOLD);
        waddch(w[MANA_G_WIN], ' ');
        wmove(w[MANA_G_WIN], 0, 13);
        drawGuage(w[MANA_G_WIN], CYAN, BLACK, 25, (float) player->mp / playerData[playerType][P_MAX_MP]);
        drawStatus(w[STAT_WIN]);
        drawEquipment(w[WEAPON_WIN], WEAPON);
        drawEquipment(w[ARMORY_WIN], ARMORY);
        drawEquipment(w[POTION_WIN], POTION);
        drawControlAid(w[CONTROL_WIN]);
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
    int lvl = playerData[playerType][P_LEVEL];

    mvwprintw(win, 1, 2, "%s", getPlayerName(playerType));
    mvwprintw(win, 3, 2, "Level: %d (Exp. %d/%d)", lvl, playerData[playerType][P_EXP], getExpCap(lvl));
    mvwprintw(win, 4, 2, "Coin: %d", inv.coin);
}

static void drawGuage(WINDOW* win, Color color_fg, Color color_bg, int length, float ratio) {
    int value = floorf(ratio * length);
    char str[length + 1];
    int cur_x, cur_y;

    getyx(win, cur_y, cur_x);
    memset(str, ' ', sizeof(char) * (length + 1));
    str[length] = '\0';

    wattron(win, COLOR_PAIR(color_fg));
    waddnstr(win, str, value);
    wattroff(win, COLOR_PAIR(color_fg));

    wattron(win, COLOR_PAIR(color_bg));
    mvwaddnstr(win, cur_y, cur_x + value, str, length - value);
    wattroff(win, COLOR_PAIR(color_bg));
    wmove(win, cur_y, cur_x - value);
}

static void drawEquipment(WINDOW* win, ItemCategory category) {
    const char *name = NULL;
    const char *title = NULL;
    int gItem = inv.equipment[category];

    if (gItem > -1) {
        name = getItemName(gItem);
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
    mvwprintw(win, 4, 2, "%s", name ? name : "Not equipped.");
}

static void drawControlAid(WINDOW* win) {
    mvwprintw(win, 1, 9, "Control Keys");
    mvwprintw(win, 2, 2, "Move: WASD");
    mvwprintw(win, 2, 20, "Attack: K");
    mvwprintw(win, 3, 2, "Skill: [SPACE]");
    mvwprintw(win, 3, 20, "Inventory: I");
    mvwprintw(win, 4, 2, "Potion: H (for HP) / M (for MP)");
}

static void drawTiles() {
    Entity *entity = getEntityByID(0);
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

                    if (portal) {
                        wattron(win, COLOR_PAIR(portal->color));
                        mvwaddch(win, y_, x_, 'P');
                        wattroff(win, COLOR_PAIR(portal->color));

                        if (prev_t != tile) {
                            char label[20];
                            const char* name;
                            
                            if (portal->dest == DUNGEON) {
                                name = getDungeonName(portal->dungeon);
                            }
                            else {
                                name = getStageName(portal->dest);
                            }

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
    Entity* iter;
    
    while (id < MAX_ENTITY) {
        Location loc;
        Texture skin;
        WINDOW* win;
        int color;
        int map_x, map_y, scr_x, scr_y;
        bool label = false;
        
        iter = getEntityByID(id++);

        if (!iter) continue;

        loc = iter->loc;
        skin = iter->skin;
        win = getGameWindow(WORLD_WIN);
        color = COLOR_PAIR(skin.color);

        for (map_y = 0; map_y < 9; map_y++) {
            if (iter->type[0] != PLAYER) {
                if (!getScreenCoordByTile(Y, loc.pos[1], &scr_y))
                    continue;
            
                scr_y += map_y - 4;
            }

            for (map_x = 0; map_x < 9; map_x++) {
                if (!skin.map[map_y][map_x])
                    continue;

                if (iter->type[0] == PLAYER) {
                    wattron(win, color);
                    mvwaddch(win, ctr_y + map_y - 4, ctr_x + map_x - 4, ' ');
                    wattroff(win, color);
                    continue;
                }

                if ( !(iter->type[0] == MONSTER && getScreenCoordByTile(X, loc.pos[0], &scr_x)))
                    continue;

                scr_x += map_x - 4;

                if (scr_x < column_ && scr_y < row_) {
                    wattron(win, color);
                    mvwaddch(win, scr_y, scr_x, ' ');
                    wattroff(win, color);
                }

                if (!label) {
                    label = true;
                    wmove(win, scr_y - 3, scr_x);

                    if (scr_y - 3 > init_y) {
                        if (skin.color == RED || skin.color == GREEN)
                            wattroff(win, color);

                        drawGuage(win, GREEN, RED, 10, (float) iter->health / monsterData[iter->type[1]][M_MAX_HEALTH]);
                        
                        if (skin.color == RED || skin.color == GREEN)
                            wattron(win, color);
                    }
                }
            }
        }
        
        wattroff(win, color);
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

    if (!player)
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

/**
 * Converts a Tile coordinate into Screen coordinate.
 * Pass the coordinate (in either X or Y depending on Axis A) into INPUT.
 * Then, RESULT will point to the computed screen coordinate.
 **/
static bool getScreenCoordByTile(enum Axis a, int input, int *result) {
    Entity *player = getEntityByID(0);
    int x, y;

    if (!player)
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