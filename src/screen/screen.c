// Redefine the macro to use poll def. & wide characters
#undef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncursesw/curses.h>
#include <ncursesw/menu.h>
#include <ncursesw/panel.h>
#include "header/screen.h"

ScreenMode screen_mode = TITLE_SCREEN;
PromptMode prompt_mode = PROMPT_NONE;
PromptMode hid_prompt_mode = PROMPT_NONE;
int column = 224;
int row = 66;
Prompt prompt = {0, 0, 0, 0, 0, 0, NULL};
static MENU *menu = NULL;
static PANEL *prompt_pan[2] = { NULL }; // 0: Background, 1: Buttons

static void updatePrompt(int);
static Prompt getPrompt(PromptMode);
static void onDialogueConfirm(ItemEventBus);

void drawScreen() {
	resizeterm(row, column);
    clear();
    border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    refresh();

    switch (screen_mode) {
        case TITLE_SCREEN:
            drawTitleScreen();
            break;
        case GAME_SCREEN:
            initGameScreen();
            break;
    }
}

void updateScreen(int key) {
    if (prompt_mode != PROMPT_NONE) {
        updatePrompt(key);
    }

    if (screen_mode == GAME_SCREEN) {
        drawGameScreen();
    }
}

void clearScreen() {
    if (screen_mode == GAME_SCREEN) {
        clearGameScreen();
    }
}

/* Draws prompt. CURSOR indicates the index of current item. */
void drawPrompt(int cursor) {
    WINDOW *win0, *win1;
    ITEM* item;

    item = prompt.items[cursor];
    menu = new_menu(prompt.items);
    win0 = newwin(prompt.height, prompt.width, prompt.y, prompt.x);
    win1 = derwin(win0, item_count(menu), 20, prompt.desc_lines + 3, 10);
    prompt_pan[0] = new_panel(win0);
    prompt_pan[1] = new_panel(win1);

    keypad(win0, TRUE);
    set_menu_win(menu, win0);
    set_menu_sub(menu, win1);
    set_menu_mark(menu, "*");
    set_current_item(menu, item);
    menu_opts_off(menu, O_SHOWDESC);
    box(win0, 0, 0);
    post_menu(menu);
    refresh();
    wrefresh(win0);
    wrefresh(win1);
    update_panels();
}

void deletePrompt() {
    if (menu == NULL) {
        return;
    }

    ITEM** items = menu_items(menu);
    WINDOW* win[2] = {getPromptWindow(0), getPromptWindow(1)};
    int count = item_count(menu);
    
    touchwin(win[0]);
    wclear(win[0]);
    wrefresh(win[0]);
    wrefresh(win[1]);
    // unpost_menu() must precede delwin() to make sure that they're unlinked
    unpost_menu(menu);
    free_menu(menu);
    del_panel(prompt_pan[0]);
    del_panel(prompt_pan[1]);
    delwin(win[0]);
    delwin(win[1]);

    for (int i = 0; i < count; i++) {
        free_item(items[i]);
    }

    items = NULL;
    menu = NULL;
    prompt_pan[0] = NULL;
    prompt_pan[1] = NULL;
    prompt_mode = PROMPT_NONE;
}

static Prompt getPrompt(PromptMode mode) {
    switch (mode) {
        case TITLE_PROMPT:
            return getTitlePrompt();
        case TITLE_CHARACTER_PROMPT:
            return getCharPrompt();
        case MOD_PROMPT:
            return getModPrompt();
        case MOD_CATEGORY_PROMPT:
            return getModCategoryPrompt();
        case INV_CATEGORY_PROMPT:
            return getCategoryPrompt();
        case INVENTORY_PROMPT:
            return getInventoryPrompt();
        case DIALOGUE_PROMPT: {
            ITEM** items;
            Prompt p;
    
            items = calloc(2, sizeof(ITEM*));
            items[0] = new_item("Confirm", "confirm");
            items[1] = NULL;
            set_item_userptr(items[0], onDialogueConfirm);

            p.desc_lines = 5;
            p.btn_length = 10;
            p.items = items;
            p.height = 15;
            p.width = 80;
            p.y = 10;
            p.x = column / 2 - 40;
            return p;
        }
    }
}

MENU* getPromptMenu() {
    return menu;
}

/* 0: Background, 1: Button box */
WINDOW* getPromptWindow(int index) {
    PANEL* pan = prompt_pan[index];

    if (pan == NULL) return NULL;
    
    return panel_window(pan);
}

void setPromptMode(PromptMode mode) {
    if (mode == DIALOGUE_PROMPT) {
        hid_prompt_mode = prompt_mode;
    }

    deletePrompt();

    if (mode != PROMPT_NONE) {
        prompt_mode = mode;
        prompt = getPrompt(mode);
        drawPrompt(0);
    }
}

void setScreenMode(ScreenMode mode) {
    deletePrompt();
    clearScreen();
    screen_mode = mode;
    drawScreen();
}

void setMenuOptions(Menu_Options options, bool on) {
    if (menu == NULL)
        return;

    if (on) {
        menu_opts_on(menu, options);
    } else {
        menu_opts_off(menu, options);
    }
}

void refreshPrompt(int cursor) {
    PromptMode mode = prompt_mode;

    setScreenMode(screen_mode);  
    prompt_mode = mode;
    prompt = getPrompt(mode);
    drawPrompt(cursor);
}

static void updatePrompt(int key) {
    ITEM* item;
    ItemEventBus bus;
    void (* buttonFunc)(ItemEventBus);
    int id;

    if (menu == NULL)
        return;

    item = current_item(menu);
    id = item_index(item);
    buttonFunc = (void (*)(ItemEventBus)) item_userptr(item);
    bus.event = HOVER;

    if (key == KEY_DOWN && id < item_count(menu) - 1) {
        refreshPrompt(id + 1);
        item = current_item(menu);
        id = item_index(item);
        buttonFunc = (void (*)(ItemEventBus)) item_userptr(item);
    }
    else if (key == KEY_UP && id > 0) {
        refreshPrompt(id - 1);
        item = current_item(menu);
        id = item_index(item);
        buttonFunc = (void (*)(ItemEventBus)) item_userptr(item);
    }
    else if (((int) item_opts(item) & O_SELECTABLE) == O_SELECTABLE) {
        switch (key) {
            case 10:
                bus.event = CLICK;
                break;
            case ' ':
                bus.event = SELECT;
                break;
        }
    }

    bus.item = item;
    touchwin(getPromptWindow(0));
    (* buttonFunc)(bus);
    wrefresh(getPromptWindow(0));
    wrefresh(getPromptWindow(1));
}

static void onDialogueConfirm(ItemEventBus bus) {
    if (bus.event == CLICK) {
        setPromptMode(hid_prompt_mode);
        hid_prompt_mode = PROMPT_NONE;
    }
}