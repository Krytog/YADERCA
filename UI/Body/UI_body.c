#include "UI_body.h"

#include <ncurses.h>
#include <stdlib.h>

#define ARROW "->"
#define NO_ARROW "  "

enum Offsets {
    HEIGHT = 3, NAME_L = 3, MODIFIED_R = 13, SIZE_R = 34
};

enum Colors {
    SELECTED = 5,
};

static void init_colors() {
    init_pair(SELECTED, COLOR_RED, COLOR_MAGENTA);
}

void init_UI_body(WINDOW **ptr, int terminal_width, int terminal_height) {
    init_colors();
    int height = terminal_height - 2 * (HEIGHT - 1);
    *ptr = newwin(height, terminal_width, HEIGHT - 1, 0);
    box(*ptr, 0, 0);
    mvwaddch(*ptr, 0, 0, ACS_LTEE);
    mvwaddch(*ptr, 0, terminal_width - 1, ACS_RTEE);
    mvwaddch(*ptr, height - 1, 0, ACS_LTEE);
    mvwaddch(*ptr, height - 1, terminal_width - 1, ACS_RTEE);
}

static void show_entities_name(WINDOW *ptr, int cur, int lines, int width, char **names, size_t size) {
    int beg_line = cur - lines + 1 > 0 ? lines - cur + 1 : 0;
    for (int i = beg_line; i < size; ++i) {
        if (i == cur) {
            mvwprintw(ptr, 1 + i, 1, ARROW);
            wattron(ptr, A_BOLD);
            mvwprintw(ptr, 1 + i, NAME_L, "%s", names[i]);
            wattroff(ptr, A_BOLD);
        } else {
            mvwprintw(ptr, 1 + i, 1, NO_ARROW);
            mvwprintw(ptr, 1 + i, NAME_L, "%s", names[i]);
        }
    }
}

void update_UI_body(WINDOW *ptr, int terminal_width, int terminal_height, const InfoHolder *info) {
    int height = terminal_height - 2 * (HEIGHT - 1);
    show_entities_name(ptr, info->selected_line, height, terminal_height, info->names, info->entities_num);
}
void destroy_UI_body(WINDOW *ptr) {
    delwin(ptr);
}