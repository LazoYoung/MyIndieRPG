#ifndef FILE_SCREEN_INCLUDED
#define FILE_SCREEN_INCLUDED

#include <ncursesw/menu.h>

typedef enum ItemEvent {
    HOVER, SELECT, CLICK
} ItemEvent;

typedef enum ScreenMode {
    TITLE_SCREEN, GAME_SCREEN, INVENTORY_SCREEN
} ScreenMode; 

typedef enum PromptMode {
    PROMPT_NONE, TITLE_PROMPT, TITLE_CHARACTER_PROMPT,
    INV_CATEGORY_PROMPT, INVENTORY_PROMPT
} PromptMode;

typedef struct {
    int width; // box width
    int height; // box height
    int x; // box coord-x
    int y; // box coord-y
    int desc_lines; // Number of rows reserved in description window
    ITEM** items;
} Prompt;
extern ScreenMode screen_mode;
extern PromptMode prompt_mode;
extern int column, row;
extern Prompt prompt;

/* screen.c */
extern void drawScreen();
extern void clearScreen();
extern void deletePrompt();
extern void drawPrompt();
extern void refreshScreen(int);
extern void setScreen(ScreenMode);
extern void setPrompt(PromptMode);
extern MENU* getPromptMenu();
extern void setMenuOptions(Menu_Options, bool);

/* title-screen.c */
extern void drawTitleScreen();

/* game-screen.c */
extern void initGameScreen();
extern void initGameResolution();
extern void drawGameScreen();
extern void clearGameScreen();

/* inventory-screen.c */
extern void drawInventoryScreen();

#endif