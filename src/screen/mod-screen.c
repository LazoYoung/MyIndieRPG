// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdlib.h>
#include <string.h>
#include <menu.h>
#include <panel.h>
#include <ncursesw/curses.h>
#include "header/screen.h"
#include "header/game.h"
#include "header/data.h"

enum ModPage {
    GAME_CATEGORY, PLAYER_CATEGORY, MONSTER_CATEGORY, ITEM_CATEGORY, DUNGEON_CATEGORY,
    PLAYER_MOD, ITEM_MOD, MONSTER_MOD, DUNGEON_MOD, RESOLUTION_MOD
} page;
static MonsterType monsterType;
static ItemType itemType;
static DungeonType dungeonType;
extern int column, row;

static void onModCategory(ItemEventBus);
static void onModInteger(ItemEventBus);
static void onReturn(ItemEventBus);
static ITEM* newAttributeItem(char *alias, int attr);

Prompt getModCategoryPrompt() {
    Prompt p;
    ITEM **items;

    if (page == GAME_CATEGORY) {
        items = calloc(7, sizeof(ITEM*));
        items[0] = new_item("Player", "Player attributes");
        items[1] = new_item("Items", "Item attributes");
        items[2] = new_item("Monsters", "Monster attributes");
        items[3] = new_item("Dungeon", "Dungeon specifications");
        items[4] = new_item("Resolution", "Screen resolution");
        items[5] = new_item("← Go back", "back");
        items[6] = NULL;
        set_item_userptr(items[0], onModCategory);
        set_item_userptr(items[1], onModCategory);
        set_item_userptr(items[2], onModCategory);
        set_item_userptr(items[3], onModCategory);
        set_item_userptr(items[4], onModCategory);
        set_item_userptr(items[5], onReturn);
    }
    else if (page == PLAYER_CATEGORY) {
        items = calloc(PT_SIZE + 2, sizeof(ITEM*));
        items[KIRITO] = new_item("Kirito", intToString(KIRITO));
        items[ASUNA] = new_item("Asuna", intToString(ASUNA));
        items[KLEIN] = new_item("Klein", intToString(KLEIN));
        items[AGIL] = new_item("Agil", intToString(AGIL));
        items[PT_SIZE] = new_item("← Go back", "back");
        items[PT_SIZE + 1] = NULL;
        set_item_userptr(items[KIRITO], onModCategory);
        set_item_userptr(items[ASUNA], onModCategory);
        set_item_userptr(items[KLEIN], onModCategory);
        set_item_userptr(items[AGIL], onModCategory);
        set_item_userptr(items[PT_SIZE], onReturn);
    }

    p.width = 50;
    p.height = 30;
    p.x = column / 2 - 25;
    p.y = row / 2 - 15;
    p.desc_lines = 3;
    p.items = items;
    return p;
}

Prompt getModPrompt() {
    Prompt p;
    ITEM **items;

    if (page == PLAYER_MOD) {
        items = calloc(6, sizeof(ITEM*));
        items[0] = newAttributeItem("Max health", P_MAX_HEALTH);
        items[1] = newAttributeItem("Max mana", P_MAX_MP);
        items[2] = newAttributeItem("Agility", P_AGI);
        items[3] = newAttributeItem("Strength", P_STR);
        items[4] = new_item("← Go back", "back");
        items[5] = NULL;
        set_item_userptr(items[0], onModInteger);
        set_item_userptr(items[1], onModInteger);
        set_item_userptr(items[2], onModInteger);
        set_item_userptr(items[3], onModInteger);
        set_item_userptr(items[4], onReturn);
    }

    p.width = 50;
    p.height = 30;
    p.x = column / 2 - 25;
    p.y = row / 2 - 15;
    p.desc_lines = 3;
    p.items = items;
    return p;
}

void initModPage() {
    page = GAME_CATEGORY;
}

static void onModCategory(ItemEventBus bus) {
    WINDOW *win = getPromptWindow(0);
    int index = item_index(bus.item);
    const char *desc = item_description(bus.item);

    if (bus.event == HOVER) {
        if (strcmp(desc, "back") != 0)
            mvwprintw(win, 3, 3, "Category: %s", desc);

        return;
    }
    else if (bus.event == CLICK) {
        if (page == GAME_CATEGORY) {
            switch (index) {
                case 0:
                    page = PLAYER_CATEGORY;
                    break;
                case 1:
                    page = ITEM_CATEGORY;
                    break;
                case 2:
                    page = MONSTER_CATEGORY;
                    break;
                case 3:
                    page = DUNGEON_CATEGORY;
                    break;
                case 4:
                    page = RESOLUTION_MOD;
                    break;
            }

            setPromptMode(MOD_CATEGORY_PROMPT);
            return;
        }

        if (page == PLAYER_CATEGORY) {
            page = PLAYER_MOD;
            playerType = atoi(item_description(bus.item));
        }
        else if (page == ITEM_CATEGORY) {
            page = ITEM_MOD;
            itemType = atoi(item_description(bus.item));
        }
        else if (page == MONSTER_CATEGORY) {
            page = MONSTER_MOD;
            monsterType = atoi(item_description(bus.item));
        }
        else if (page == DUNGEON_CATEGORY) {
            page = DUNGEON_MOD;
            dungeonType = atoi(item_description(bus.item));
        }

        setPromptMode(MOD_PROMPT);
    }
}

static void onModInteger(ItemEventBus bus) {
    WINDOW *win = getPromptWindow(0);
    int attr = atoi(item_description(bus.item));
    char input[5];
    int value, y, x;

    if (bus.event == HOVER) {
        mvwprintw(win, 1, 2, "Hit <ENTER> to modify the attribute.");
        return;
    }

    mvwprintw(win, 3, 2, "Wrong value is converted into the minimum.");
    mvwprintw(win, 4, 2, "Type the numeric value: _____");
    getyx(win, y, x);
    wmove(win, y, x - 5);
    curs_set(1);
    wattron(win, WA_BLINK);
    echo();
    wrefresh(win);
    wgetnstr(win, input, sizeof(char) * 5);
    noecho();
    attroff(WA_BLINK);
    curs_set(0);
    value = atoi(input);

    switch (attr) {
        case P_MAX_HEALTH:
        case P_MAX_MP:
            value = value > 0 ? value : 1;
            break;
        case P_AGI:
        case P_STR:
            value = value >= 0 ? value : 0;
            break;
    }

    switch (page) {
        case PLAYER_MOD:
            playerAttr[playerType][attr] = value;
            break;
    }

    setPromptMode(MOD_PROMPT);
}

static void onReturn(ItemEventBus bus) {
    if (bus.event != CLICK) return;

    switch (page) {
        case GAME_CATEGORY:
            setPromptMode(TITLE_PROMPT);
            return;
        
        case PLAYER_CATEGORY:
        case ITEM_CATEGORY:
        case MONSTER_CATEGORY:
        case DUNGEON_CATEGORY:
            page = GAME_CATEGORY;
            break;
        
        case PLAYER_MOD:
            page = PLAYER_CATEGORY;
            break;
        case ITEM_MOD:
            page = ITEM_CATEGORY;
            break;
        case MONSTER_MOD:
            page = MONSTER_CATEGORY;
            break;
        case DUNGEON_MOD:
            page = DUNGEON_CATEGORY;
            break;
    }

    setPromptMode(MOD_CATEGORY_PROMPT);
}

static ITEM* newAttributeItem(char *alias, int attr) {
    char* name;
    int value;

    switch (page) {
        case PLAYER_MOD:
            value = playerAttr[playerType][attr];
            break;
    }

    name = calloc(40, sizeof(char));
    sprintf(name, "%s: %d", alias, value);
    return new_item(name, intToString(attr));
}