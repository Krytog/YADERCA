#include "UI_Legendl.h"
#include "../colors.h"

#include <ncurses.h>

enum Offsets {
    HEIGHT = 3, NAME_L = 3, MODIFIED_R = 13, SIZE_R = 34
};

static void init_colors() {
    init_pair(DEFAULT_COLOR, COLOR_YELLOW, -1);
}

void init_UI_legendl(WINDOW **ptr, int terminal_width) {
    *ptr = newwin(HEIGHT, terminal_width, 0, 0);
    box(*ptr, 0, 0);
    init_colors();
    wattron(*ptr, COLOR_PAIR(DEFAULT_COLOR));
    mvwprintw(*ptr, 1, NAME_L, "Name");
    mvwprintw(*ptr, 1, terminal_width - MODIFIED_R, "Modified");
    mvwprintw(*ptr, 1, terminal_width - SIZE_R, "Size");
    wattroff(*ptr, COLOR_PAIR(DEFAULT_COLOR));
}

void destroy_UI_legendl(WINDOW *ptr) {
    delwin(ptr);
}