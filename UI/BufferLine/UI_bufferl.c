#include "UI_bufferl.h"
#include "../colors.h"

#include <ncurses.h>
#include <string.h>

#define VERSION "YADERCA v1.3"

#define NON_KEYWORD "NONE"
#define COPY_KEYWORD "CPY->"
#define CUT_KEYWORD "CUT->"

enum {
    HEIGHT = 3, LEFT_OFFSET = 10, RIGHT_OFFSET = 15, NAME_OFFSET = 15
};

static void init_colors() {
    init_pair(SPECIAL_BUFFER, COLOR_MAGENTA, -1);
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

static void clear_buffer_line(WINDOW *ptr, int len) {
    if (len == -1) {
        return;
    }
    char *temp = calloc(len + 1, sizeof(*temp));
    if (!temp) {
        return;
    }
    for (int i = 0; i < len; ++i) {
        temp[i] = ' ';
    }
    temp[len] = '\0';
    mvwprintw(ptr, 1, LEFT_OFFSET, "%s", temp);
    free(temp);
}

void update_UI_bufferl(WINDOW *ptr, const InfoHolder *info_holder) {
    static int prev_len = -1;
    clear_buffer_line(ptr, prev_len);
    switch (info_holder->buffer.mode) {
        case BUFFER_MODE_EMPTY: {
            wattron(ptr, COLOR_PAIR(EMPTY_BUFFER));
            mvwprintw(ptr, 1, LEFT_OFFSET, NON_KEYWORD);
            wattroff(ptr, COLOR_PAIR(EMPTY_BUFFER));
            break;
        }
        case BUFFER_MODE_COPY: {
            wattron(ptr, COLOR_PAIR(SPECIAL_BUFFER));
            mvwprintw(ptr, 1, LEFT_OFFSET, COPY_KEYWORD);
            mvwprintw(ptr, 1, NAME_OFFSET, "%s", info_holder->buffer.name);
            wattroff(ptr, COLOR_PAIR(SPECIAL_BUFFER));
            prev_len = strlen(info_holder->buffer.name) + strlen(COPY_KEYWORD) + 1;
            break;
        }
        case BUFFER_MODE_CUT: {
            wattron(ptr, COLOR_PAIR(SPECIAL_BUFFER));
            mvwprintw(ptr, 1, LEFT_OFFSET, CUT_KEYWORD);
            mvwprintw(ptr, 1, NAME_OFFSET, "%s", info_holder->buffer.name);
            wattroff(ptr, COLOR_PAIR(SPECIAL_BUFFER));
            prev_len = strlen(info_holder->buffer.name) + strlen(CUT_KEYWORD) + 1;
            break;
        }
    }
}

void destroy_UI_bufferl(WINDOW *ptr) {
    delwin(ptr);
}