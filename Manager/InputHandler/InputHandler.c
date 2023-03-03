#include "InputHandler.h"
#include "../Traveler/traveler.h"
#include <ncurses.h>

enum Commands {
    COMMAND_EXIT = 'q', COMMAND_INTO = '\n', COMMAND_DELETE1 = 'd', COMMAND_DELETE2 = 'D',
    COMMAND_SWITCH_HIDDEN_SHOW1 = 'h', COMMAND_SWITCH_HIDDEN_SHOW2 = 'H',
    COMMAND_COPY1 = 'c', COMMAND_COPY2 = 'C',
    COMMAND_INSERT1 = 'v', COMMAND_INSERT2 = 'V',
    COMMAND_CUT1 = 'x', COMMAND_CUT2 = 'X',
    COMMAND_DEATH = 'T'
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
        case COMMAND_DELETE1:
        case COMMAND_DELETE2: {
            delete_file(path_holder, get_selected_name(info), info, active);
            break;
        }
        case COMMAND_SWITCH_HIDDEN_SHOW1:
        case COMMAND_SWITCH_HIDDEN_SHOW2: {
            switch_show_hidden(info);
            break;
        }
        case COMMAND_COPY1:
        case COMMAND_COPY2: {
            add_to_buffer(path_holder, get_selected_name(info), info, active, BUFFER_MODE_COPY);
            break;
        }
        case COMMAND_CUT1:
        case COMMAND_CUT2: {
            add_to_buffer(path_holder, get_selected_name(info), info, active, BUFFER_MODE_CUT);
            break;
        }
        case COMMAND_INSERT1:
        case COMMAND_INSERT2: {
            clever_insert_from_buffer(path_holder, info, active);
            break;
        }
        case COMMAND_INTO: {
            try_open_entity(path_holder, get_selected_name(info), info, active);
            break;
        }
    }
}