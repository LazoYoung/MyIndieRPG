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

extern int column, row;

extern void initModPage();
extern void suspend();
static void printAttributes();
static void onStart(ItemEventBus);
static void onMod(ItemEventBus);
static void onQuit(ItemEventBus);
static void onSelectCharacter(ItemEventBus);
static void onReturn(ItemEventBus);

void drawTitleScreen() {
    mvaddch(10, column / 2 - 8, ACS_DIAMOND);
    printw(" My Indie RPG ");
    addch(ACS_DIAMOND);
    mvprintw(12, column / 2 - 14, "RPG Prototype Simulating SAO");
    mvprintw(13, column / 2 - 18, "2019136063 parkcymil@koreatech.ac.kr");
}

Prompt getTitlePrompt() {
    Prompt p;
    ITEM **items;
    
    items = calloc(4, sizeof(ITEM*));
    items[0] = new_item("◎ Start Game", "start");
    items[1] = new_item("◎ Modification", "mod");
    items[2] = new_item("◎ Quit", "quit");
    items[3] = NULL; // ITEM* array has to be terminated with NULL element.

    // Inject function pointers to their respective items.
    // These functions define the functionality of menu item.
    set_item_userptr(items[0], onStart);
    set_item_userptr(items[1], onMod);
    set_item_userptr(items[2], onQuit);

    p.width = 40;
    p.height = 15;
    p.x = column / 2 - 20;
    p.y = row / 2 - 5;
    p.desc_lines = 2;
    p.items = items;
    return p;
}

Prompt getCharPrompt() {
    Prompt p;
    ITEM **items;
    
    items = (ITEM**) calloc(6, sizeof(ITEM*));
    items[0] = new_item("▣ Kirito", intToString(KIRITO));
    items[1] = new_item("▣ Asuna", intToString(ASUNA));
    items[2] = new_item("▣ Klein", intToString(KLEIN));
    items[3] = new_item("▣ Agil", intToString(AGIL));
    items[4] = new_item("← Go back", "back");
    items[5] = NULL;

    set_item_userptr(items[0], onSelectCharacter);
    set_item_userptr(items[1], onSelectCharacter);
    set_item_userptr(items[2], onSelectCharacter);
    set_item_userptr(items[3], onSelectCharacter);
    set_item_userptr(items[4], onReturn);

    p.width = 40;
    p.height = 20;
    p.x = column / 2 - 20;
    p.y = row / 2 - 10;
    p.desc_lines = 8;
    p.items = items;
    return p;
}

static void onStart(ItemEventBus bus) {
    if (bus.event == CLICK) {
        setPromptMode(TITLE_CHARACTER_PROMPT);
        return;
    }

    mvwprintw(getPromptWindow(0), 2, 3, "Hit <Enter> to start the game!");
}

static void onMod(ItemEventBus bus) {
    if (bus.event == CLICK) {
        initModPage();
        setPromptMode(MOD_CATEGORY_PROMPT);
        return;
    }

    mvwprintw(getPromptWindow(0), 2, 3, "Hit <Enter> to modify game data.");
}

static void onQuit(ItemEventBus bus) {
    if (bus.event == CLICK) {
        suspend();
        return;
    }
    
    mvwprintw(getPromptWindow(0), 2, 3, "Hit <Enter> to exit the game.");
}

static void onSelectCharacter(ItemEventBus bus) {
    switch (atoi(item_description(bus.item))) {
        case KIRITO:
            playerType = KIRITO;
            break;
        case ASUNA:
            playerType = ASUNA;
            break;
        case KLEIN:
            playerType = KLEIN;
            break;
        case AGIL:
            playerType = AGIL;
            break;
    }

    if (bus.event == CLICK) {
        startGame();
    } else {
        printAttributes();        
    }
}

static void onReturn(ItemEventBus bus) {
    if (bus.event == CLICK) {
        setPromptMode(TITLE_PROMPT);
    }
}

static void printAttributes() {
    WINDOW *w = getPromptWindow(0);
    int r = 4;
    mvwprintw(w, 1, 3, "Character - %s", getPlayerName(playerType));
    mvwprintw(w, 3, 3, "HP: %d", playerData[playerType][P_MAX_HEALTH]);
    mvwprintw(w, 4, 3, "MP: %d", playerData[playerType][P_MAX_MP]);
    mvwprintw(w, 5, 3, "AGILITY: %d", playerData[playerType][P_AGI]);
    mvwprintw(w, 6, 3, "STRENGTH: %d", playerData[playerType][P_STR]);

    if (hasSkill(DUAL_WIELD)) {
        mvwprintw(w, r++, 3, "UNIQUE SKILL: Dual Wield");
    }
    if (hasSkill(AXE_BERSERK)) {
        mvwprintw(w, r++, 3, "UNIQUE SKILL: Axe Berserk");
    }
    if (hasSkill(DOUBLE_JUMP)) {
        mvwprintw(w, r++, 3, "MISC SKILL: Double Jump");
    }
    if (hasSkill(EXP_BONUS)) {
        mvwprintw(w, r++, 3, "MISC SKILL: EXP Bonus");
    }
    if (hasSkill(HEALTH_RECOVERY)) {
        mvwprintw(w, r++, 3, "MISC SKILL: HP Recovery");
    }
    if (hasSkill(MANA_RECOVERY)) {
        mvwprintw(w, r++, 3, "MISC SKILL: MP Recovery");
    }
}