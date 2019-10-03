#include <menu.h>

// 화면 모드를 정의하는 플래그
#define TITLE_SCREEN 0

// 화면 메뉴를 정의하는 플래그
#define TITLE_PROMPT 0

typedef struct prompt {
    int width;
    int height;
    ITEM** items;
} Prompt;

// screen.c
extern void drawScreen(short);
extern void deletePrompt();
extern void drawPrompt(Prompt);

// title-screen.c
extern void drawTitleScreen();