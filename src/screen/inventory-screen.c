// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncursesw/curses.h>
#include <ncursesw/panel.h>
#include "header/screen.h"
#include "header/game.h"
#include "header/data.h"

static ItemCategory category;

static void onSelectCategory(ItemEventBus);
static void onSelectItem(ItemEventBus);
static void onReturn(ItemEventBus);

Prompt getCategoryPrompt() {
    ITEM** items;
    Prompt prompt;
    ItemEventBus bus;

    items = calloc(6, sizeof(ITEM*));
    items[0] = new_item("◇ Weapon", "weapon");
    items[1] = new_item("◇ Armory", "armory");
    items[2] = new_item("◇ Potion", "potion");
    items[3] = new_item("◇ Coin", "coin");
    items[4] = new_item("× Close Menu", "close");
    items[5] = NULL;
    item_opts_off(items[3], O_SELECTABLE);
    
    set_item_userptr(items[0], onSelectCategory);
    set_item_userptr(items[1], onSelectCategory);
    set_item_userptr(items[2], onSelectCategory);
    set_item_userptr(items[3], onSelectCategory);
    set_item_userptr(items[4], onSelectCategory);

    prompt.width = 50;
    prompt.height = 20;
    prompt.x = column / 2 - 25;
    prompt.y = row / 2 - 10;
    prompt.desc_lines = 1;
    prompt.items = items;
    return prompt;
}

Prompt getInventoryPrompt() {
    ITEM** items = calloc(SLOT_CAP, sizeof(ITEM*));
    Prompt prompt;
    int n = 0;

    for (int i = 0; i < SLOT_CAP; i++) {
        int itemType = inv.items[category][i];

        if (itemType < 0) continue;

        items[n] = new_item(getItemName(itemType), intToString(itemType));
        set_item_userptr(items[n], onSelectItem);
        n++;
    }

    items = realloc(items, (n + 2) * sizeof(ITEM*));
    items[n] = new_item("← Go back", "back");
    items[n + 1] = NULL;
    set_item_userptr(items[n], onReturn);

    prompt.width = 50;
    prompt.height = 20;
    prompt.x = column / 2 - 25;
    prompt.y = row / 2 - 10;
    prompt.desc_lines = 8;
    prompt.items = items;
    return prompt;
}

static void onSelectCategory(ItemEventBus bus) {
    const char* desc = item_description(bus.item);

    if (bus.event == CLICK) {
        if (strcmp(desc, "weapon") == 0) {
            category = WEAPON;
            setPromptMode(INVENTORY_PROMPT);
        }
        else if (strcmp(desc, "armory") == 0) {
            category = ARMORY;
            setPromptMode(INVENTORY_PROMPT);
        }
        else if (strcmp(desc, "potion") == 0) {
            category = POTION;
            setPromptMode(INVENTORY_PROMPT);
        }
        else if (strcmp(desc, "close") == 0) {
            setScreenMode(GAME_SCREEN);
        }
        else {
            setPromptMode(INV_CATEGORY_PROMPT);
        }
    }
    else if (bus.event == HOVER) {
        WINDOW *win = menu_win(getPromptMenu());
        wattron(win, A_BOLD);
        mvwprintw(win, 1, 16, "Inventory");
        wattroff(win, A_BOLD);

        if (strcmp(desc, "coin") == 0) {
            mvwprintw(win, 2, 3, "Your coin: %d", inv.coin);
        }
        else if (strcmp(desc, "close") == 0) {
            mvwprintw(win, 2, 3, "Hit <ENTER> to close inventory.");
        }
        else {
            mvwprintw(win, 2, 3, "Hit <ENTER> to display your %s items.", item_description(bus.item));
        }
    }
}

static void onSelectItem(ItemEventBus bus) {
    WINDOW *win = getPromptWindow(0);
    ItemType gItem = atoi(item_description(bus.item));
    const char *name = getItemName(gItem);
    int *equip = &inv.equipment[category];
    
    switch (bus.event) {
        case CLICK: {
            if (*equip == gItem) {
                *equip = -1;
            }
            else {
                *equip = gItem;
            }
            
            setPromptMode(INVENTORY_PROMPT);
            break;
        }

        case HOVER: {
            char desc[30];
            int value = itemAttr[gItem][I_VALUE];

            switch (itemAttr[gItem][I_CATEGORY]) {
                case WEAPON:
                    sprintf(desc, "Attack power: %d", value);
                    break;
                case ARMORY:
                    sprintf(desc, "Damage absorbtion: %d", value);
                    break;
                case POTION:
                    if (gItem == HEAL_CRYSTAL) {
                        sprintf(desc, "HP recovery: %d", value);
                    }
                    else if (gItem == MANA_CRYSTAL) {
                        sprintf(desc, "MP recovery: %d", value);                        
                    }
                    break;
            }

            mvwprintw(win, 2, 3, "Name: %s", name);
            mvwprintw(win, 3, 3, desc);
            mvwprintw(win, 4, 3, "Equipped: %s", *equip == gItem ? "YES" : "NO");
            break;
        }
    }
}

static void onReturn(ItemEventBus bus) {
    if (bus.event == CLICK) {
        setPromptMode(INV_CATEGORY_PROMPT);
    }
}