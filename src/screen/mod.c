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
    PLAYER_MOD, MONSTER_MOD, ITEM_MOD, DUNGEON_MOD, RESOLUTION_MOD
} page;
enum DataCategory {
    MONSTER_DATA, ITEM_DATA, ITEM_CATEGORY_DATA, DUNGEON_DATA
};
static MonsterType monsterType;
static ItemType itemType;
static DungeonType dungeonType;
extern int column, row;

static void onModCategory(ItemEventBus);
static void onModInteger(ItemEventBus);
static void onReturn(ItemEventBus);
static ITEM* newIntegerItem(char *alias, int attr);
static ITEM* newCategoryItem(char *alias, int attr, enum DataCategory category);

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
    else {
        int size, i;
        char* (*getLabel)(int);

        switch (page) {
            case PLAYER_CATEGORY:
                size = PT_SIZE;
                getLabel = getPlayerName;
                break;
            case MONSTER_CATEGORY:
                size = MT_SIZE;
                getLabel = getMonsterName;
                break;
            case ITEM_CATEGORY:
                size = IT_SIZE;
                getLabel = getItemName;
                break;
            case DUNGEON_CATEGORY:
                size = DT_SIZE;
                getLabel = getDungeonName;
                break;
        }

        items = calloc(size + 2, sizeof(ITEM*));

        for (i = 0; i < size; i++) {
            items[i] = new_item((*getLabel)(i), intToString(i));
            set_item_userptr(items[i], onModCategory);
        }
        
        items[i] = new_item("← Go back", "back");
        items[i + 1] = NULL;
        set_item_userptr(items[i], onReturn);
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
        items = calloc(8, sizeof(ITEM*));
        items[0] = newIntegerItem("Max health", P_MAX_HEALTH);
        items[1] = newIntegerItem("Max mana", P_MAX_MP);
        items[2] = newIntegerItem("Agility", P_AGI);
        items[3] = newIntegerItem("Strength", P_STR);
        items[4] = newIntegerItem("Level", P_LEVEL);
        items[5] = newIntegerItem("Experience", P_EXP);
        items[6] = new_item("← Go back", "back");
        items[7] = NULL;
        set_item_userptr(items[0], onModInteger);
        set_item_userptr(items[1], onModInteger);
        set_item_userptr(items[2], onModInteger);
        set_item_userptr(items[3], onModInteger);
        set_item_userptr(items[4], onModInteger);
        set_item_userptr(items[5], onModInteger);
        set_item_userptr(items[6], onReturn);
    }
    else if (page == MONSTER_MOD) {
        items = calloc(7, sizeof(ITEM*));
        items[0] = newIntegerItem("Max health", M_MAX_HEALTH);
        items[1] = newIntegerItem("Agility", M_AGI);
        items[2] = newIntegerItem("Defense", M_ABSORB);
        items[3] = newIntegerItem("Skin color", M_COLOR);
        items[4] = newIntegerItem("Volume size", M_VOLUME);
        items[5] = new_item("← Go back", "back");
        items[6] = NULL;
        set_item_userptr(items[0], onModInteger);
        set_item_userptr(items[1], onModInteger);
        set_item_userptr(items[2], onModInteger);
        set_item_userptr(items[3], onModInteger);
        set_item_userptr(items[4], onModInteger);
        set_item_userptr(items[5], onReturn);
    }
    else if (page == ITEM_MOD) {
        items = calloc(5, sizeof(ITEM*));
        items[0] = newCategoryItem("Category", I_CATEGORY, ITEM_CATEGORY_DATA);
        items[1] = newIntegerItem("Value", I_VALUE);
        items[2] = newIntegerItem("Obtained asset", I_ASSET);
        items[3] = new_item("← Go back", "back");
        items[4] = NULL;
        set_item_userptr(items[0], onModInteger);
        set_item_userptr(items[1], onModInteger);
        set_item_userptr(items[2], onModInteger);
        set_item_userptr(items[3], onReturn);
    }
    else if (page == DUNGEON_MOD) {
        items = calloc(9, sizeof(ITEM*));
        items[0] = newCategoryItem("Monster 1", D_MONSTER_1, MONSTER_DATA);
        items[1] = newCategoryItem("Monster 2", D_MONSTER_2, MONSTER_DATA);
        items[2] = newCategoryItem("Monster 3", D_MONSTER_3, MONSTER_DATA);
        items[3] = newCategoryItem("Reward 1", D_REWARD_1, ITEM_DATA);
        items[4] = newCategoryItem("Reward 2", D_REWARD_2, ITEM_DATA);
        items[5] = newCategoryItem("Reward 3", D_REWARD_3, ITEM_DATA);
        items[6] = newIntegerItem("Experience", D_EXP);
        items[7] = new_item("← Go back", "back");
        items[8] = NULL;
        set_item_userptr(items[0], onModInteger);
        set_item_userptr(items[1], onModInteger);
        set_item_userptr(items[2], onModInteger);
        set_item_userptr(items[3], onModInteger);
        set_item_userptr(items[4], onModInteger);
        set_item_userptr(items[5], onModInteger);
        set_item_userptr(items[6], onModInteger);
        set_item_userptr(items[7], onReturn);
    }
    else if (page == RESOLUTION_MOD) {
        items = calloc(4, sizeof(ITEM*));
        items[0] = newIntegerItem("Width", 0);
        items[1] = newIntegerItem("Height", 1);
        items[2] = new_item("← Go back", "back");
        items[3] = NULL;
        set_item_userptr(items[0], onModInteger);
        set_item_userptr(items[1], onModInteger);
        set_item_userptr(items[2], onReturn);
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
                    setPromptMode(MOD_PROMPT);
                    return;
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

    if (page == PLAYER_MOD) {
        switch (attr) {
            case P_MAX_HEALTH:
            case P_MAX_MP:
            case P_LEVEL:
                value = value > 0 ? value : 1;
                break;
            case P_AGI:
            case P_STR:
            case P_EXP:
                value = value >= 0 ? value : 0;
                break;
        }
    }
    else if (page == MONSTER_MOD) {
        switch (attr) {
            case M_VOLUME:
                value = value > 0 && value < 4 ? value : 1;
                break;
            case M_MAX_HEALTH:
            case M_DAMAGE:
                value = value > 0 ? value : 1;
                break;
            case M_ABSORB:
            case M_COLOR:
                value = value > 0 ? value : 0;
                break;
            case M_AGI:
                value = value >= -100 && value <= 100 ? value : -100;                
                break;
        }
    }
    else if (page == ITEM_MOD) {
        switch (attr) {
            case I_VALUE:
            case I_ASSET:
                value = value > 0 ? value : 0;
                break;
        }
    }
    else if (page == DUNGEON_MOD) {
        switch (attr) {
            case D_MONSTER_1:
            case D_MONSTER_2:
            case D_MONSTER_3:
                value = value >= -1 && value < MT_SIZE ? value : -1;
                break;
            case D_REWARD_1:
            case D_REWARD_2:
            case D_REWARD_3:
                value = value >= -1 && value < IT_SIZE ? value : -1;
                break;
            case D_EXP:
                value = value > 0 ? value : 0;
                break;
        }
    }
    else if (page == RESOLUTION_MOD) {
        value = value > 50 ? value : 50;
    }

    switch (page) {
        case PLAYER_MOD:
            playerData[playerType][attr] = value;
            break;
        case MONSTER_MOD:
            monsterData[monsterType][attr] = value;
            break;
        case ITEM_MOD:
            itemData[itemType][attr] = value;
            break;
        case DUNGEON_MOD:
            dungeonData[dungeonType][attr] = value;
            break;
        case RESOLUTION_MOD:
            switch (attr) {
                case 0: // Width
                    column = value;
                    break;
                case 1:
                    row = value;
                    break;
            }
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
        case RESOLUTION_MOD:
            page = GAME_CATEGORY;
            break;
    }

    setPromptMode(MOD_CATEGORY_PROMPT);
}

static ITEM* newIntegerItem(char *alias, int attr) {
    char* name;
    int value;

    switch (page) {
        case PLAYER_MOD:
            value = playerData[playerType][attr];
            break;
        case MONSTER_MOD:
            value = monsterData[monsterType][attr];
            break;
        case ITEM_MOD:
            value = itemData[itemType][attr];
            break;
        case DUNGEON_MOD:
            value = dungeonData[dungeonType][attr];
            break;
        case RESOLUTION_MOD:
            switch (attr) {
                case 0: // Width
                    value = column;
                    break;
                case 1:
                    value = row;
                    break;
            }
            initGameResolution();
            break;
    }

    name = calloc(40, sizeof(char));
    sprintf(name, "%s: %d", alias, value);
    return new_item(name, intToString(attr));
}

static ITEM* newCategoryItem(char *alias, int attr, enum DataCategory category) {
    char* name = calloc(40, sizeof(char));
    const char* attr_name;
    int attr_val;

    switch (page) {
        case PLAYER_MOD:
            attr_val = playerData[playerType][attr];
            break;
        case MONSTER_MOD:
            attr_val = monsterData[monsterType][attr];
            break; 
        case ITEM_MOD:
            attr_val = itemData[itemType][attr];
            break;
        case DUNGEON_MOD:
            attr_val = dungeonData[dungeonType][attr];
            break;
    }

    switch (category) {
        case MONSTER_DATA:
            attr_name = getMonsterName(attr_val);
            break;
        case ITEM_DATA:
            attr_name = getItemName(attr_val);
            break;
        case ITEM_CATEGORY_DATA:
            attr_name = getItemCategoryName(attr_val);
            break;
        case DUNGEON_DATA:
            attr_name = getDungeonName(attr_val);
            break;
    }

    sprintf(name, "%s: %s", alias, attr_name);
    return new_item(name, intToString(attr));
}