#include "UI_bufferl.h"

#include <ncurses.h>

#define VERSION "YADERCA v0.6"

enum {
    HEIGHT = 3, LEFT_OFFSET = 10, RIGHT_OFFSET = 15
};

enum Colors {
    DEFAULT_COLOR = 2, EMPTY_BUFFER = 3
};

static void init_colors() {
    init_pair(DEFAULT_COLOR, COLOR_YELLOW, -1);
    init_pair(EMPTY_BUFFER, COLOR_RED, -1);
}

void init_UI_bufferl(WINDOW **ptr, int terminal_width, int terminal_height) {
    *ptr = newwin(HEIGHT, terminal_width, terminal_height - HEIGHT, 0);
    box(*ptr, 0, 0);
    init_colors();
    wattron(*ptr, COLOR_PAIR(DEFAULT_COLOR));
    mvwprintw(*ptr, 1, 2, "BUFFER:");
    mvwprintw(*ptr, 1, terminal_width - RIGHT_OFFSET, VERSION);
    wattroff(*ptr, COLOR_PAIR(DEFAULT_COLOR));
}

void update_UI_bufferl(WINDOW *ptr, int *future) {
    wattron(ptr, COLOR_PAIR(EMPTY_BUFFER));
    mvwprintw(ptr, 1, LEFT_OFFSET, "NONE");
    wattron(ptr, COLOR_PAIR(EMPTY_BUFFER));
}
void destroy_UI_bufferl(WINDOW *ptr) {
    delwin(ptr);
}