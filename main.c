#include <ncurses.h>
#include <unistd.h>

#include "UI/UI.h"
#include "Manager/InputHandler/InputHandler.h"
#include "Manager/Traveler/traveler.h"

int main() {
    initscr();
    init_UI();

    InfoHolder info_holder;
    info_holder_init(&info_holder);

    PathHolder path_holder;
    path_holder_init(&path_holder);

    int active = 1;
    keypad(stdscr, TRUE);

    go_into_dir(&path_holder, NULL, &info_holder, &active);

    while (active == 1) {
        draw_UI(&info_holder);
        wait_for_and_handle_input(&path_holder, &info_holder, &active);
    }

    destroy_UI();
    info_holder_clean_up(&info_holder);

    endwin();
    return 0;
}