#include "UI.h"
#include "BufferLine/UI_bufferl.h"

#include <ncurses.h>

typedef struct TerminalSize {
    int width;
    int height;
} TerminalSize;

TerminalSize terminal_size;

typedef struct UIptrs {
    WINDOW *buffer_line;
} UIptrs;

UIptrs UI_ptrs;

TerminalSize get_terminal_size() {
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
    init_UI_bufferl(&UI_ptrs.buffer_line, terminal_size.width, terminal_size.height);
}

void destroy_UI() {
    destroy_UI_bufferl(UI_ptrs.buffer_line);
}

void refresh_all() {
    update_UI_bufferl(UI_ptrs.buffer_line, NULL);

    refresh();
    wrefresh(UI_ptrs.buffer_line);
}

void draw_UI(int *future) {
    refresh_all();
}