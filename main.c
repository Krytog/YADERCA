#include <ncurses.h>
#include <stdlib.h>

#include "UI/UI.h"
#include "Manager/InputHandler/InputHandler.h"

int main() {
    initscr();
    init_UI();

    InfoHolder info_holder;
    char *names[4];
    names[0] = "jklfefa";
    names[1] = "FKLGSHGJG";
    names[3] = "ABOBA";
    names[2] = "jklfgqjopjioewgegrjio";
    info_holder.names = names;
    info_holder.selected_line = 0;
    info_holder.entities_num = 4;

    int active = 1;
    keypad(stdscr, TRUE);
    while (active) {
        draw_UI(&info_holder);
        wait_for_and_handle_input(&info_holder, &active);
    }
    destroy_UI();
    endwin();
    return 0;
}