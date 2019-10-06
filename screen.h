#include <menu.h>

// 화면 모드를 정의하는 플래그
#define TITLE_SCREEN 0

// 프롬프트 메뉴 모드를 정의하는 플래그
#define PROMPT_NONE 0
#define TITLE_PROMPT 1
#define TITLE_CHARACTER_PROMPT 2

typedef struct prompt_str {
    int width; // box width
    int height; // box height
    int x; // box coord-x
    int y; // box coord-y
    int desc_lines; // Number of rows reserved in description window
    ITEM** items;
} Prompt;

extern short screen_mode;
extern short prompt_mode;
extern int column, row;
extern Prompt prompt;

// screen.c
extern void drawScreen();
extern void deletePrompt();
extern void drawPrompt();
extern void refreshPrompt(int);
extern void toggleScreen(short);
extern void togglePrompt(short);
extern WINDOW* getPromptWindow();

// title-screen.
extern void drawTitleScreen();