// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <ncursesw/curses.h>
#include <ncursesw/panel.h>
#include "header/screen.h"
#include "header/game.h"
#include "header/level.h"

enum WindowType {
    HEALTH_WIN, HEALTH_G_WIN, MANA_WIN, MANA_G_WIN,
    WEAPON_WIN, ARMORY_WIN, POTION_WIN, CONTROL_WIN, WORLD_WIN
};

static PANEL* panels[9];
static int row_, column_, init_y, init_x, ctr_y, ctr_x;

static void drawGuage(WINDOW*, const char*, Color, Color, char);
static void drawEquipment(WINDOW*, ItemCategory);
static void drawControlAid(WINDOW*);
static void drawTiles();
static void drawEntities();
static WINDOW* getWindow(enum WindowType);

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
    WINDOW *weapon_win = newwin(5, 20, init_y + row_, init_x + 1);
    WINDOW *armory_win = newwin(5, 20, init_y + row_, init_x + 23);
    WINDOW *potion_win = newwin(5, 20, init_y + row_, init_x + 45);
    WINDOW *ctrl_win = newwin(5, 30, init_y + row_, column_ - 30);
    panels[WORLD_WIN] = new_panel(world_win);
    panels[HEALTH_WIN] = new_panel(hp_win_0);
    panels[MANA_WIN] = new_panel(mp_win_0);
    panels[HEALTH_G_WIN] = new_panel(hp_win_1);
    panels[MANA_G_WIN] = new_panel(mp_win_1);
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

    generateLevel();
}

void drawGameScreen() {
    WINDOW* health_bg = getWindow(HEALTH_WIN);
    WINDOW* health_w = getWindow(HEALTH_G_WIN);
    WINDOW* mana_bg = getWindow(MANA_WIN);
    WINDOW* mana_w = getWindow(MANA_G_WIN);
    WINDOW* weapon_w = getWindow(WEAPON_WIN);
    WINDOW* armory_w = getWindow(ARMORY_WIN);
    WINDOW* potion_w = getWindow(POTION_WIN);
    WINDOW* ctrl_w = getWindow(CONTROL_WIN);

    for (int i = 0; i <= WORLD_WIN; i++) {
        WINDOW *w = getWindow(i);

        if (w != NULL)
            werase(w);
    }

    box(health_bg, ACS_VLINE, ACS_HLINE);
    box(mana_bg, ACS_VLINE, ACS_HLINE);
    box(weapon_w, ACS_VLINE, ACS_HLINE);
    box(armory_w, ACS_VLINE, ACS_HLINE);
    box(potion_w, ACS_VLINE, ACS_HLINE);
    box(ctrl_w, ACS_VLINE, ACS_HLINE);

    if (inGame) {
        drawGuage(health_w, "Health", GREEN, RED, p_attr.health);
        drawGuage(mana_w, "Mana", CYAN, BLACK, p_attr.mp);
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

void clearGameScreen() { // TODO Clear all the windows
    WINDOW* w;

    for (int i=0; i<=WORLD_WIN; i++) {
        w = getWindow(i);
        wclear(w);
        del_panel(panels[i]);
        delwin(w);
    }

    destructLevel();
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
    mvwprintw(win, 1, 2, "Move: WASD");
    mvwprintw(win, 2, 2, "Attack: K");
    mvwprintw(win, 1, 15, "Use Item: H");
    mvwprintw(win, 2, 15, "Inventory: I");
}

static void drawTiles() {
    Entity entity = *getEntity(p_attr.name);
    WINDOW *win = getWindow(WORLD_WIN);
    Location loc;

    if (entity.valid) {
        loc = entity.loc;
    }

    werase(win);
    box(win, ACS_VLINE, ACS_HLINE);
    
    // Convert coordinates from screen to tile
    for (int y_ = 0; y_ < row_; y_++) {
        int y = loc.pos[1] + ctr_y - y_;
        Tile prev_t = AIR;

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
    Entity* iter = getEntity(NULL);
    int cnt = 0;
    
    while (iter && iter->valid) {
        Location loc = iter->loc;
        Texture skin = iter->skin;
        int color = COLOR_PAIR(10);
        WINDOW* win = getWindow(WORLD_WIN);
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
        mvwprintw(win, 10 + cnt++, 2, "%s's position: %.2f %.2f", iter->name, iter->loc.pos[0], iter->loc.pos[1]);
        mvwprintw(win, 10 + cnt++, 2, "%s's speed: %.2f %.2f", iter->name, iter->loc.spd[0], iter->loc.spd[1]);

        iter = iter->next;
    }
}

static WINDOW* getWindow(enum WindowType type) {
    if (panels[type] == NULL)
        return NULL;

    return panel_window(panels[type]);
}