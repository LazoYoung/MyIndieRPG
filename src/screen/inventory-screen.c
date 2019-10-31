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

static ItemCategory category;

static void onSelectCategory(ItemEvent, ITEM*);
static void onSelectItem(ItemEvent, ITEM*);
static void onReturn(ItemEvent, ITEM*);

void drawInventoryScreen() {
    drawPrompt();
}

Prompt getCategoryPrompt() {
    ITEM** items;
    Prompt prompt;

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
    ITEM** items = calloc(10, sizeof(ITEM*));
    Prompt prompt;
    int n = 0;

    for (int i=0; i<10; i++) {
        GItem gitem = inv.items[i];

        if (gitem.valid && gitem.category == category) {
            static char desc[4];
            snprintf(desc, 4, "%d", i);

            gitem.id_str = desc;
            items[n] = new_item(getItemName(gitem.type), desc);
            set_item_userptr(items[n], onSelectItem);
            n++;
        }
    }

    items = realloc(items, n + 1);
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

static void onSelectCategory(ItemEvent event, ITEM* item) {
    WINDOW *win = menu_win(getPromptMenu());
    const char* desc = item_description(item);

    if (event == CLICK) {
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
            setPromptMode(PROMPT_NONE);
            setScreenMode(GAME_SCREEN);
        }
        else {
            setPromptMode(INV_CATEGORY_PROMPT);
        }
    }
    else if (event == HOVER) {
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
            mvwprintw(win, 2, 3, "Hit <ENTER> to display your %s items.", item_description(item));
        }
    }
}

static void onSelectItem(ItemEvent event, ITEM* item) {
    WINDOW *win = menu_win(getPromptMenu());
    int id = atoi(item_description(item));    
    GItem *gitem = &inv.items[id];
    const char *name = getItemName(gitem->type);
    
    switch (event) {
        case CLICK: {
            if (gitem->equip) {
                gitem->equip = false;
                inv.equipment[category] = NULL;
            }
            else {
                if (inv.equipment[category] != NULL) {
                    mvwprintw(win, 2, 3, "You can't have multiple items");
                    mvwprintw(win, 3, 3, "equipped in the same category!");
                    break;
                }

                inv.equipment[category] = gitem;
                gitem->equip = true;
            }

            setPromptMode(INVENTORY_PROMPT);
            break;
        }

        case HOVER: {
            if (gitem->valid) {
                mvwprintw(win, 2, 3, "Name: %s", name);
                mvwprintw(win, 3, 3, "Equipped: %s", gitem->equip ? "YES" : "NO");
            }
            break;
        }
    }
}

static void onReturn(ItemEvent event, ITEM* item) {
    if (event == CLICK) {
        setPromptMode(INV_CATEGORY_PROMPT);
    }
}