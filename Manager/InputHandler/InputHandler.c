#include "InputHandler.h"

#include <ncurses.h>

enum {
    COMMAND_INTO = '\n'
};

void wait_for_and_handle_input(InfoHolder *info, int *active) {
    int input = getch();
    switch (input) {
        case 'q': {
            input = getch();
            if (input == 'q') {
                *active = 0;
            }
            break;
        }
        case KEY_DOWN: {
            if (info->selected_line + 1 < info->entities_num) {
                ++info->selected_line;
            }
            break;
        }
        case KEY_UP: {
            if (info->selected_line > 0) {
                --info->selected_line;
            }
            break;
        }
        case COMMAND_INTO: {
            *active = 0;
            break;
        }
    }
}