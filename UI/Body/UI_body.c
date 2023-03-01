#include "UI_body.h"

#include <ncurses.h>
#include <stdlib.h>

#define ARROW "->"
#define NO_ARROW "  "

#define SELECTED_STYLE COLOR_PAIR(SELECTED) | A_BOLD

enum Offsets {
    HEIGHT = 3, NAME_L = 3, MODIFIED_R = 13, SIZE_R = 34
};

enum Colors {
    SELECTED = 5,
};

enum {
    SPECIFIER_NUM_SIZE = 7
};

static char specifier[SPECIFIER_NUM_SIZE];

static char *selected_background;

static void init_colors() {
    init_pair(SELECTED, COLOR_WHITE, COLOR_BLUE);
}

static void init_specifier(int width) {
    int max_length = width - SIZE_R - NAME_L - 1;
    sprintf(specifier + 2, "%ds", max_length);
    specifier[0] = '%';
    specifier[1] = '.';
}

void init_selected_background(int width) {
    selected_background = calloc(width - NAME_L, sizeof(*selected_background));
    for (int i = 0; i < width - NAME_L - 1; ++i) {
        selected_background[i] = ' ';
    }
}

static void init_UI_corners(WINDOW *ptr, int width, int height) {
    mvwaddch(ptr, 0, 0, ACS_LTEE);
    mvwaddch(ptr, 0, width - 1, ACS_RTEE);
    mvwaddch(ptr, height - 1, 0, ACS_LTEE);
    mvwaddch(ptr, height - 1, width - 1, ACS_RTEE);
}

void init_UI_body(WINDOW **ptr, int terminal_width, int terminal_height) {
    init_colors();
    init_specifier(terminal_width);
    init_selected_background(terminal_width);
    int height = terminal_height - 2 * (HEIGHT - 1);
    *ptr = newwin(height, terminal_width, HEIGHT - 1, 0);
    box(*ptr, 0, 0);
    init_UI_corners(*ptr, terminal_width, height);
}

static void show_entities_name(WINDOW *ptr, int cur, int lines, const Entity *entities, size_t size) {
    int beg_line = cur - lines + 1 > 0 ? lines - cur + 1 : 0;
    for (int i = beg_line; i < size; ++i) {
        if (i == cur) {
            mvwprintw(ptr, 1 + i, 1, ARROW);
            wattron(ptr, SELECTED_STYLE);
            mvwprintw(ptr, 1 + i, NAME_L, specifier, entities[i].name);
            wattroff(ptr, SELECTED_STYLE);
        } else {
            mvwprintw(ptr, 1 + i, 1, NO_ARROW);
            mvwprintw(ptr, 1 + i, NAME_L, specifier, entities[i].name);
        }
    }
}

static void show_entities_size(WINDOW *ptr, int cur, int lines, int width, const Entity *entities, size_t size) {
    int beg_line = cur - lines + 1 > 0 ? lines - cur + 1 : 0;
    for (int i = beg_line; i < size; ++i) {
        if (i == cur) {
            wattron(ptr, SELECTED_STYLE);
            mvwprintw(ptr, 1 + i, width - SIZE_R, "%zu", entities[i].size);
            wattroff(ptr, SELECTED_STYLE);
        } else {
            mvwprintw(ptr, 1 + i, width - SIZE_R, "%zu", entities[i].size);
        }
    }
}

static void show_entities_date(WINDOW *ptr, int cur, int lines, int width, const Entity *entities, size_t size) {
    int beg_line = cur - lines + 1 > 0 ? lines - cur + 1 : 0;
    for (int i = beg_line; i < size; ++i) {
        if (i == cur) {
            wattron(ptr, SELECTED_STYLE);
            mvwprintw(ptr, 1 + i, width - MODIFIED_R, "%s", entities[i].date);
            wattroff(ptr, SELECTED_STYLE);
        } else {
            mvwprintw(ptr, 1 + i, width - MODIFIED_R, "%s", entities[i].date);
        }
    }
}

static void highlight_line(WINDOW *ptr, int cur) {
    wattron(ptr, SELECTED_STYLE);
    mvwprintw(ptr, 1 + cur, NAME_L, "%s", selected_background);
    wattroff(ptr, SELECTED_STYLE);
}

static void clear_line(WINDOW *ptr, int cur) {
    if (cur == -1) {
        return;
    }
    mvwprintw(ptr, 1 + cur, NAME_L, "%s", selected_background);
}

void update_UI_body(WINDOW *ptr, int terminal_width, int terminal_height, const InfoHolder *info) {
    static int prev_highlight = -1;
    int height = terminal_height - 2 * (HEIGHT - 1);
    clear_line(ptr, prev_highlight);
    highlight_line(ptr, info->selected_line);
    show_entities_name(ptr, info->selected_line, height, info->entities, info->entities_num);
    show_entities_size(ptr, info->selected_line, height, terminal_width, info->entities, info->entities_num);
    show_entities_date(ptr, info->selected_line, height, terminal_width, info->entities, info->entities_num);
    prev_highlight = info->selected_line;
}
void destroy_UI_body(WINDOW *ptr) {
    delwin(ptr);
    free(selected_background);
}