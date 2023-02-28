#include <ncurses.h>

#include "UI/UI.h"

int main() {
    initscr();
    init_UI();
    int status = 1;
    while (status) {
        draw_UI(NULL);
        int command = getch();
        if (command == 'e') {
            status = 0;
        }
    }
    destroy_UI();
    endwin();
    return 0;
}