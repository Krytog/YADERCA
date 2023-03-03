#include "UI_body.h"

#include <ncurses.h>
#include <stdlib.h>
#include <dirent.h>

#define ARROW "->"
#define NO_ARROW "  "

enum Offsets {
    HEIGHT = 3, NAME_L = 3, MODIFIED_R = 13, SIZE_R = 34
};

#define SELECTED_STYLE COLOR_PAIR(SELECTED_FILE) | A_BOLD

enum Colors {
    SELECTED_FILE = 5, CASUAL_FILE = 6,
    SELECTED_DIR = 7, CASUAL_DIR = 8,
    SELECTED_OTHER = 9, CASUAL_OTHER = 10
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
    init_pair(SELECTED_FILE, COLOR_WHITE, COLOR_BLUE);
    init_pair(CASUAL_FILE, COLOR_WHITE, -1);
    init_pair(SELECTED_DIR, COLOR_MAGENTA, COLOR_BLUE);
    init_pair(CASUAL_DIR, COLOR_MAGENTA, -1);
    init_pair(SELECTED_OTHER, COLOR_RED, COLOR_BLUE);
    init_pair(CASUAL_OTHER, COLOR_RED, COLOR_BLUE);
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

static int get_style(unsigned type, int selected) {
    if (selected) {
        switch (type) {
            case DT_REG: {
                return COLOR_PAIR(SELECTED_FILE) | A_BOLD;
            }
            case DT_DIR: {
                return COLOR_PAIR(SELECTED_DIR) | A_BOLD;
            }
            default: {
                return COLOR_PAIR(SELECTED_OTHER) | A_BOLD;
            }
        }
    }
    switch (type) {
        case DT_REG: {
            return COLOR_PAIR(CASUAL_FILE);
        }
        case DT_DIR: {
            return COLOR_PAIR(CASUAL_DIR);
        }
        default: {
            return COLOR_PAIR(CASUAL_OTHER);
        }
    }
}

static void show_entities_name(WINDOW *ptr, int cur, const Entity *entities, const size_t *indices, int show_hidden) {
    if (show_hidden) {
        for (int i = begin; i < end; ++i) {
            int style = get_style(entities[i].type, i == cur);
            mvwprintw(ptr, 1 + i - begin, 1, i == cur ? ARROW : NO_ARROW);
            wattron(ptr, style);
            mvwprintw(ptr, 1 + i - begin, NAME_L, specifier, entities[i].name);
            wattroff(ptr, style);
        }
        return;
    }
    for (int i = begin; i < end; ++i) {
        int style = get_style(entities[indices[i]].type, i == cur);
        mvwprintw(ptr, 1 + i - begin, 1, i == cur ? ARROW : NO_ARROW);
        wattron(ptr, style);
        mvwprintw(ptr, 1 + i - begin, NAME_L, specifier, entities[indices[i]].name);
        wattroff(ptr, style);
    }
}

static void show_entities_size(WINDOW *ptr, int cur, int width, const Entity *entities, const size_t *indices, int show_hidden) {
    if (show_hidden) {
        for (int i = begin; i < end; ++i) {
            int style = get_style(entities[i].type, i == cur);
            wattron(ptr, style);
            if (entities[i].type == DT_DIR) {
                mvwprintw(ptr, 1 + i - begin, width - SIZE_R, "DIR");
            } else {
                mvwprintw(ptr, 1 + i - begin, width - SIZE_R, "%zu", entities[i].size);
            }
            wattroff(ptr, style);
        }
        return;
    }
    for (int i = begin; i < end; ++i) {
        int style = get_style(entities[indices[i]].type, i == cur);
        wattron(ptr, style);
        if (entities[i].type == DT_DIR) {
            mvwprintw(ptr, 1 + i - begin, width - SIZE_R, "DIR");
        } else {
            mvwprintw(ptr, 1 + i - begin, width - SIZE_R, "%zu", entities[indices[i]].size);
        }
        wattroff(ptr, style);
    }
}

static void show_entities_date(WINDOW *ptr, int cur, int width, const Entity *entities, const size_t *indices, int show_hidden) {
    if (show_hidden) {
        for (int i = begin; i < end; ++i) {
            int style = get_style(entities[i].type, i == cur);
            wattron(ptr, style);
            mvwprintw(ptr, 1 + i - begin, width - MODIFIED_R, "%s", entities[i].date);
            wattroff(ptr, style);
        }
        return;
    }
    for (int i = begin; i < end; ++i) {
        int style = get_style(entities[indices[i]].type, i == cur);
        wattron(ptr, style);
        mvwprintw(ptr, 1 + i - begin, width - MODIFIED_R, "%s", entities[indices[i]].date);
        wattroff(ptr, style);
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
    if (info->show_hidden) {
        set_begin_and_end(info->selected_line, height, info->entities_num);
    } else {
        set_begin_and_end(info->selected_line, height, info->not_hidden_info.num);
    }
    clear_before_update(ptr, height);
    highlight_line(ptr, info->selected_line);
    show_entities_name(ptr, info->selected_line, info->entities, info->not_hidden_info.indices, info->show_hidden);
    show_entities_size(ptr, info->selected_line, terminal_width, info->entities, info->not_hidden_info.indices, info->show_hidden);
    show_entities_date(ptr, info->selected_line, terminal_width, info->entities, info->not_hidden_info.indices, info->show_hidden);
}

void destroy_UI_body(WINDOW *ptr) {
    delwin(ptr);
    free(selected_background);
}
