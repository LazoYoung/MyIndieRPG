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
static char desc[INVENTORY_CAP][4];

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
    prompt.gitems = NULL;
    prompt.gitem_count = 0;
    return prompt;
}

Prompt getInventoryPrompt() {
    ITEM** items = calloc(INVENTORY_CAP, sizeof(ITEM*));
    GItem** gitems = calloc(INVENTORY_CAP, sizeof(GItem*));
    Prompt prompt;
    int n = 0;

    for (int i = 0; i < INVENTORY_CAP; i++) {
        GItem *gitem = inv.items[i];

        if (gitem && gitem->category == category) {
            items[n] = new_item(getItemName(gitem->type), NULL);
            gitems[n] = gitem;
            set_item_userptr(items[n], onSelectItem);
            n++;
        }
    }

    items = realloc(items, (n + 2) * sizeof(ITEM*));
    gitems = realloc(gitems, n * sizeof(GItem*));
    items[n] = new_item("← Go back", "back");
    items[n + 1] = NULL;
    set_item_userptr(items[n], onReturn);

    prompt.width = 50;
    prompt.height = 20;
    prompt.x = column / 2 - 25;
    prompt.y = row / 2 - 10;
    prompt.desc_lines = 8;
    prompt.items = items;
    prompt.gitems = gitems;
    prompt.gitem_count = n;
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
    GItem *gitem = bus.gitem;
    const char *name = getItemName(gitem->type);
    
    switch (bus.event) {
        case CLICK: {
            if (gitem->equip) {
                gitem->equip = false;
                inv.equipment[category] = NULL;
            }
            else {
                if (inv.equipment[category] != NULL) {
                    WINDOW *win1;

                    setPromptMode(DIALOGUE_PROMPT);
                    win1 = getPromptWindow(0);

                    mvwprintw(win1, 5, 3, "You can't have multiple items");
                    mvwprintw(win1, 6, 3, "equipped in the same category!");
                    break;
                }

                inv.equipment[category] = gitem;
                gitem->equip = true;
            }

            setPromptMode(INVENTORY_PROMPT);
            break;
        }

        case HOVER: {
            char desc[20];

            switch (gitem->category) {
                case WEAPON:
                    sprintf(desc, "Attack power: %d", gitem->value);
                    break;
                case ARMORY:
                    sprintf(desc, "Defense level: %d", gitem->value);
                    break;
                case POTION: // TODO Potion item value determines whether HP or MP.
                    sprintf(desc, "%s recovery", gitem->value == 0 ? "HP" : "MP");
                    break;
            }

            mvwprintw(win, 2, 3, "Name: %s", name);
            mvwprintw(win, 3, 3, desc);
            mvwprintw(win, 4, 3, "Equipped: %s", gitem->equip ? "YES" : "NO");
            break;
        }
    }
}

static void onReturn(ItemEventBus bus) {
    if (bus.event == CLICK) {
        setPromptMode(INV_CATEGORY_PROMPT);
    }
}