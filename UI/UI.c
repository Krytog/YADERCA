#include "UI.h"
#include "BufferLine/UI_bufferl.h"
#include "LegendLine/UI_Legendl.h"

#include <ncurses.h>

typedef struct TerminalSize {
    int width;
    int height;
} TerminalSize;

static TerminalSize terminal_size;

typedef struct UIptrs {
    WINDOW *legend_line;
    WINDOW *buffer_line;
} UIptrs;

static UIptrs UI_ptrs;

static TerminalSize get_terminal_size() {
    TerminalSize output;
    getmaxyx(stdscr, output.height, output.width);
    return output;
}

void init_UI() {
    curs_set(0);
    noecho();
    start_color();
    use_default_colors();
    terminal_size = get_terminal_size();
    init_UI_legendl(&UI_ptrs.legend_line, terminal_size.width);
    init_UI_bufferl(&UI_ptrs.buffer_line, terminal_size.width, terminal_size.height);
}

void destroy_UI() {
    destroy_UI_legendl(UI_ptrs.legend_line);
    destroy_UI_bufferl(UI_ptrs.buffer_line);
}

static void refresh_all() {
    // legend line can't update
    update_UI_bufferl(UI_ptrs.buffer_line, NULL);
    refresh();
    wrefresh(UI_ptrs.legend_line);
    wrefresh(UI_ptrs.buffer_line);
}

void draw_UI(int *future) {
    refresh_all();
}