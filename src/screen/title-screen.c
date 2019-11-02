// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdlib.h>
#include <menu.h>
#include <panel.h>
#include <ncursesw/curses.h>
#include "header/screen.h"
#include "header/game.h"

extern int column, row;

extern void suspend();
static void printAttributes();
static void onStart(ItemEventBus);
static void onQuit(ItemEventBus);
static void onKirito(ItemEventBus);
static void onAsuna(ItemEventBus);
static void onKlein(ItemEventBus);
static void onAgil(ItemEventBus);
static void onReturn(ItemEventBus);

void drawTitleScreen() {
    mvaddch(10, column / 2 - 8, ACS_DIAMOND);
    printw(" My Indie RPG ");
    addch(ACS_DIAMOND);
    mvprintw(12, column / 2 - 14, "RPG Prototype Simulating SAO");
    mvprintw(13, column / 2 - 18, "2019136063 parkcymil@koreatech.ac.kr");
}

Prompt getTitlePrompt() {
    ITEM **myItems = calloc(3, sizeof(ITEM*));
    myItems[0] = new_item("◎ Start Game", "Start Game");
    myItems[1] = new_item("◎ Quit", "Quit");
    myItems[2] = NULL; // 말단 원소는 NULL값으로 지정해야 함

    // 버튼 동작 함수를 NCurse로 넘겨준다.
    set_item_userptr(myItems[0], onStart);
    set_item_userptr(myItems[1], onQuit);

    Prompt p;
    p.width = 40;
    p.height = 15;
    p.x = column / 2 - 20;
    p.y = row / 2 - 5;
    p.desc_lines = 1;
    p.items = myItems;
    p.gitems = NULL;
    p.gitem_count = 0;
    return p;
}

Prompt getCharPrompt() {
    Prompt p;
    ITEM **myItems = (ITEM**) calloc(6, sizeof(ITEM*));

    myItems[0] = new_item("▣ Kirito", "Kirito");
    myItems[1] = new_item("▣ Asuna", "Asuna");
    myItems[2] = new_item("▣ Klein", "Klein");
    myItems[3] = new_item("▣ Agil", "Agil");
    myItems[4] = new_item("← Go back", "Go back");
    myItems[5] = NULL;

    set_item_userptr(myItems[0], onKirito);
    set_item_userptr(myItems[1], onAsuna);
    set_item_userptr(myItems[2], onKlein);
    set_item_userptr(myItems[3], onAgil);
    set_item_userptr(myItems[4], onReturn);

    p.width = 40;
    p.height = 20;
    p.x = column / 2 - 20;
    p.y = row / 2 - 10;
    p.desc_lines = 8;
    p.items = myItems;
    p.gitems = NULL;
    p.gitem_count = 0;
    return p;
}

/* 시작 버튼을 누른 이벤트 */
static void onStart(ItemEventBus bus) {
    if (bus.event == CLICK) {
        setPromptMode(TITLE_CHARACTER_PROMPT);
        return;
    }

    mvwprintw(menu_win(getPromptMenu()), 1, 3, "Hit <Enter> to select your character!");
}

/* 종료 버튼을 누른 이벤트 */
static void onQuit(ItemEventBus bus) {
    if (bus.event == CLICK) {
        suspend();
        return;
    }
    
    mvwprintw(menu_win(getPromptMenu()), 1, 3, "Hit <Enter> to exit the game.");
}

static void onKirito(ItemEventBus bus) {
    p_attr.max_health = 100;
    p_attr.max_mp = 100;
    p_attr.agility = 30;
    p_attr.strength = 30;
    p_attr.name = "Kirito";
    inv.skills = 0;
    assignSkill(MANA_RECOVERY);
    assignSkill(DUAL_WIELD);

    if (bus.event == CLICK) {
        startGame();
    } else {
        printAttributes();        
    }
}

static void onAsuna(ItemEventBus bus) {
    p_attr.max_health = 100;
    p_attr.max_mp = 100;
    p_attr.agility = 50;
    p_attr.strength = 20;
    p_attr.name = "Asuna";
    inv.skills = 0;
    assignSkill(HEALTH_RECOVERY);

    if (bus.event == CLICK) {
        startGame();
    } else {
        printAttributes();        
    }
}

static void onKlein(ItemEventBus bus) {
    p_attr.max_health = 100;
    p_attr.max_mp = 100;
    p_attr.agility = 30;
    p_attr.strength = 30;
    p_attr.name = "Klein";
    inv.skills = 0;
    assignSkill(MANA_RECOVERY);
    assignSkill(EXP_BONUS);

    if (bus.event == CLICK) {
        startGame();
    } else {
        printAttributes();        
    }
}

static void onAgil(ItemEventBus bus) {
    p_attr.max_health = 100;
    p_attr.max_mp = 100;
    p_attr.agility = 20;
    p_attr.strength = 50;
    p_attr.name = "Agil";
    inv.skills = 0;
    assignSkill(AXE_BERSERK);

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
    WINDOW *w = menu_win(getPromptMenu());
    int r = 4;
    mvwprintw(w, 1, 3, "Name: %s", p_attr.name);
    mvwprintw(w, 2, 3, "AGILITY: %d", p_attr.agility);
    mvwprintw(w, 3, 3, "STRENGTH: %d", p_attr.strength);

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