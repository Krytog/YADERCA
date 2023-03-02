#include "UI_body.h"

#include <ncurses.h>
#include <stdlib.h>
#include <dirent.h>

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

int begin = -1;
int end = -1;
int cur_size = -1;

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

static void set_begin_and_end(int cur, int lines, int size) {
    if (cur_size != size) {
        begin = 0;
        end = lines < size ? lines : size;
        cur_size = size;
        return;
    }
    if (cur < begin) {
        --begin;
        --end;
        return;
    }
    if (cur == end) {
        ++begin;
        ++end;
        return;
    }
}

static void show_entities_name(WINDOW *ptr, int cur, const Entity *entities) {
        for (int i = begin; i < end; ++i) {
        if (i == cur) {
            mvwprintw(ptr, 1 + i - begin, 1, ARROW);
            wattron(ptr, SELECTED_STYLE);
            mvwprintw(ptr, 1 + i - begin, NAME_L, "%s", entities[i].name);
            wattroff(ptr, SELECTED_STYLE);
        } else {
            mvwprintw(ptr, 1 + i - begin, 1, NO_ARROW);
            mvwprintw(ptr, 1 + i - begin, NAME_L, specifier, entities[i].name);
        }
    }
}

static void show_entities_size(WINDOW *ptr, int cur, int width, const Entity *entities) {
    for (int i = begin; i < end; ++i) {
        if (i == cur) {
            wattron(ptr, SELECTED_STYLE);
            if (entities[i].type == DT_DIR) {
                mvwprintw(ptr, 1 + i - begin, width - SIZE_R, "DIR");
            } else {
                mvwprintw(ptr, 1 + i - begin, width - SIZE_R, "%zu", entities[i].size);
            }
            wattroff(ptr, SELECTED_STYLE);
        } else {
            if (entities[i].type == DT_DIR) {
                mvwprintw(ptr, 1 + i - begin, width - SIZE_R, "DIR");
            } else {
                mvwprintw(ptr, 1 + i - begin, width - SIZE_R, "%zu", entities[i].size);
            }
        }
    }
}

static void show_entities_date(WINDOW *ptr, int cur, int width, const Entity *entities) {
    for (int i = begin; i < end; ++i) {
        if (i == cur) {
            wattron(ptr, SELECTED_STYLE);
            mvwprintw(ptr, 1 + i - begin, width - MODIFIED_R, "%s", entities[i].date);
            wattroff(ptr, SELECTED_STYLE);
        } else {
            mvwprintw(ptr, 1 + i - begin, width - MODIFIED_R, "%s", entities[i].date);
        }
    }
}

static void highlight_line(WINDOW *ptr, int cur) {
    wattron(ptr, SELECTED_STYLE);
    mvwprintw(ptr, 1 + cur - begin, NAME_L, "%s", selected_background);
    wattroff(ptr, SELECTED_STYLE);
}

static void clear_line(WINDOW *ptr, int cur) {
    if (cur == -1) {
        return;
    }
    mvwprintw(ptr, 1 + cur, NAME_L, "%s", selected_background);
    mvwprintw(ptr, 1 + cur, 1, "%s", NO_ARROW);
}

static void clear_before_update(WINDOW *ptr, int height) {
    for (int i = 0; i < height; ++i) {
        clear_line(ptr, i);
    }
}

void update_UI_body(WINDOW *ptr, int terminal_width, int terminal_height, const InfoHolder *info) {
    int height = terminal_height - 2 * HEIGHT;
    set_begin_and_end(info->selected_line, height, info->entities_num);
    clear_before_update(ptr, height);
    highlight_line(ptr, info->selected_line);
    show_entities_name(ptr, info->selected_line, info->entities);
    show_entities_size(ptr, info->selected_line, terminal_width, info->entities);
    show_entities_date(ptr, info->selected_line, terminal_width, info->entities);
}

void destroy_UI_body(WINDOW *ptr) {
    delwin(ptr);
    free(selected_background);
}
