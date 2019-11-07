#include <ncursesw/curses.h>
#include <string.h>
#include <stdlib.h>
#include "header/screen.h"
#include "header/pause.h"

bool pause;

extern void suspend();
static void onPauseEvent(ItemEventBus);

Prompt getPausePrompt() {
    Prompt p;
    ITEM** items;

    items = calloc(4, sizeof(ITEM*));
    items[0] = new_item("Continue", "continue");
    items[1] = new_item("Save", "save");
    items[2] = new_item("Quit", "quit");
    items[3] = NULL;

    set_item_userptr(items[0], onPauseEvent);
    set_item_userptr(items[1], onPauseEvent);
    set_item_userptr(items[2], onPauseEvent);
    
    p.desc_lines = 1;
    p.height = 8;
    p.width = 30;
    p.y = row / 2 - 4;
    p.x = column / 2 - 15;
    p.items = items;
    return p;
}

/* @pause_ - True pauses the game, while False resumes it. */
void pauseGame(bool pause_) {
    pause = pause_;
}

static void onPauseEvent(ItemEventBus bus) {
    WINDOW* win = getPromptWindow(0);
    const char* item = item_description(bus.item);

    if (strcmp(item, "continue") == 0) {
        if (bus.event == CLICK) {
            setPromptMode(PROMPT_NONE);
            pauseGame(false);
        }
    }
    else if (strcmp(item, "save") == 0) {
        if (bus.event == HOVER) {
            mvwprintw(win, 1, 3, "Save the current progress.");
        }
        else if (bus.event == CLICK) {
            // TODO Start saving
        }
    }
    else if (strcmp(item, "quit") == 0) {
        if (bus.event == HOVER) {
            mvwprintw(win, 1, 3, "Exit the game.");
        }
        else if (bus.event == CLICK) {
            suspend();
        }
    }
}