#ifndef FILE_SCREEN_INCLUDED
#define FILE_SCREEN_INCLUDED

#include "game-struct.h"
#include <ncursesw/menu.h>

typedef enum WindowType { // WORLD_WIN is the last element
    HEALTH_WIN, HEALTH_G_WIN, MANA_WIN, MANA_G_WIN, STAT_WIN,
    WEAPON_WIN, ARMORY_WIN, POTION_WIN, CONTROL_WIN, WORLD_WIN,
    WT_SIZE
} WindowType;

typedef enum ItemEvent {
    HOVER, SELECT, CLICK
} ItemEvent;

typedef enum ScreenMode {
    TITLE_SCREEN, GAME_SCREEN
} ScreenMode; 

typedef enum PromptMode {
    PROMPT_NONE, TITLE_PROMPT, TITLE_CHARACTER_PROMPT, MOD_PROMPT, MOD_CATEGORY_PROMPT,
    INV_CATEGORY_PROMPT, INVENTORY_PROMPT, DIALOGUE_PROMPT, PAUSE_PROMPT
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
void drawScreen();
void clearScreen();
void deletePrompt();
void drawPrompt(int cursor);
void refreshPrompt(int cursor);
void updateScreen(int key);
void setScreenMode(ScreenMode mode);
void setPromptMode(PromptMode mode);
MENU* getPromptMenu();
WINDOW* getPromptWindow(int index);
void setMenuOptions(Menu_Options, bool);

/* title-screen.c */
void drawTitleScreen();
Prompt getTitlePrompt();
Prompt getCharPrompt();
Prompt getModPrompt();
Prompt getModCategoryPrompt();

/* game-screen.c */
void initGameScreen();
void initGameResolution();
void drawGameScreen();
void clearGameScreen();
WINDOW* getGameWindow(enum WindowType type);

/* inventory.c */
Prompt getCategoryPrompt();
Prompt getInventoryPrompt();

/* mod.c */
Prompt getModPrompt();
Prompt getModCategoryPrompt();

/* pause.c */
Prompt getPausePrompt();

#endif