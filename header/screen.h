#ifndef FILE_SCREEN_INCLUDED
#define FILE_SCREEN_INCLUDED

#include "game-struct.h"
#include <ncursesw/menu.h>

typedef enum WindowType { // WORLD_WIN is the last element
    HEALTH_WIN, HEALTH_G_WIN, MANA_WIN, MANA_G_WIN, STAT_WIN,
    WEAPON_WIN, ARMORY_WIN, POTION_WIN, CONTROL_WIN, WORLD_WIN
} WindowType;

typedef enum ItemEvent {
    HOVER, SELECT, CLICK
} ItemEvent;

typedef enum ScreenMode {
    TITLE_SCREEN, GAME_SCREEN
} ScreenMode; 

typedef enum PromptMode {
    PROMPT_NONE, TITLE_PROMPT, TITLE_CHARACTER_PROMPT, MOD_PROMPT, MOD_CATEGORY_PROMPT,
    INV_CATEGORY_PROMPT, INVENTORY_PROMPT, DIALOGUE_PROMPT
} PromptMode;

typedef struct {
    int width; // box width
    int height; // box height
    int x; // box coord-x
    int y; // box coord-y
    int desc_lines; // Number of rows reserved in description window
    ITEM** items;
} Prompt;

typedef struct {
    ITEM* item;
    ItemEvent event;
} ItemEventBus;

extern ScreenMode screen_mode;
extern PromptMode prompt_mode, hid_prompt_mode;
extern int column, row;
extern Prompt prompt;

/* screen.c */
extern void drawScreen();
extern void clearScreen();
extern void deletePrompt();
extern void drawPrompt(int cursor);
extern void refreshPrompt(int cursor);
extern void updateScreen(int key);
extern void setScreenMode(ScreenMode mode);
extern void setPromptMode(PromptMode mode);
extern MENU* getPromptMenu();
extern WINDOW* getPromptWindow(int index);
extern void setItemName(ITEM *item, const char* name);
extern void setMenuOptions(Menu_Options, bool);

/* title-screen.c */
extern void drawTitleScreen();
extern Prompt getTitlePrompt();
extern Prompt getCharPrompt();
extern Prompt getModPrompt();
extern Prompt getModCategoryPrompt();

/* game-screen.c */
extern void initGameScreen();
extern void initGameResolution();
extern void drawGameScreen();
extern void clearGameScreen();
extern WINDOW* getGameWindow(enum WindowType type);

/* inventory-screen.c */
extern Prompt getCategoryPrompt();
extern Prompt getInventoryPrompt();

#endif