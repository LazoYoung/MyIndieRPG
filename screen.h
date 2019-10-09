#define _GNU_SOURCE
#include <ncursesw/menu.h>

// 화면 모드를 정의하는 플래그
#define TITLE_SCREEN 0
#define GAME_SCREEN 1

// 프롬프트 메뉴 모드를 정의하는 플래그
#define PROMPT_NONE 10
#define TITLE_PROMPT 11
#define TITLE_CHARACTER_PROMPT 12

typedef struct prompt_str {
    int width; // box width
    int height; // box height
    int x; // box coord-x
    int y; // box coord-y
    int desc_lines; // Number of rows reserved in description window
    ITEM** items;
} Prompt;

extern char screen_mode;
extern char prompt_mode;
extern int column, row;
extern Prompt prompt;

// screen.c
extern void drawScreen();
extern void deletePrompt();
extern void drawPrompt();
extern void refreshScreen(int);
extern void toggleScreen(char);
extern void togglePrompt(char);
extern WINDOW* getPromptWindow();

// title-screen.c
extern void drawTitleScreen();

// game-screen.c
extern void drawGameScreen();