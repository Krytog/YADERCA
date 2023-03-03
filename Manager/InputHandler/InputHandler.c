#include "InputHandler.h"
#include "../Traveler/traveler.h"
#include <ncurses.h>

enum Commands {
    COMMAND_EXIT = 'q', COMMAND_INTO = '\n', COMMAND_DELETE1 = 'D', COMMAND_DELETE2 = 'd',
    COMMAND_SWITCH_HIDDEN_SHOW1 = 'h', COMMAND_SWITCH_HIDDEN_SHOW2 = 'H'
};

void wait_for_and_handle_input(PathHolder *path_holder, InfoHolder *info, int *active) {
    int input = getch();
    switch (input) {
        case COMMAND_EXIT: {
            input = getch();
            if (input == COMMAND_EXIT) {
                *active = 0;
            }
            break;
        }
        case KEY_DOWN: {
            increase_selected_line(info);
            break;
        }
        case KEY_UP: {
            decrease_selected_line(info);
            break;
        }
        case COMMAND_DELETE1: {
            delete_file(path_holder, get_selected_name(info), info, active);
            break;
        }
        case COMMAND_DELETE2: {
            delete_file(path_holder, get_selected_name(info), info, active);
            break;
        }
        case COMMAND_SWITCH_HIDDEN_SHOW1: {
            switch_show_hidden(info);
            break;
        }
        case COMMAND_SWITCH_HIDDEN_SHOW2: {
            switch_show_hidden(info);
            break;
        }
        case COMMAND_INTO: {
            go_into_dir(path_holder, get_selected_name(info), info, active);
            break;
        }
    }
}