#include <ncurses.h>
#include <stdlib.h>

#include "UI/UI.h"
#include "Manager/InputHandler/InputHandler.h"

int main() {
    initscr();
    init_UI();

    InfoHolder info_holder;
    Entity e1;
    e1.name = "aboba";
    e1.size = 555;
    e1.date = "APR 16 21:21";
    Entity e2;
    e2.name = "bebra";
    e2.size = (size_t)5535326625;
    e2.date = "JAN 10  2021";
    Entity e3;
    e3.name = "gav";
    e3.size = 1;
    e3.date = "OCT 21 22:22";
    Entity e4;
    e4.name = "eghsrhgrjgjwroighoigjiowhigowrguoiojwrgiohwrhgiorwshiogrwhiogwhiorghiowrgiohwrgohiw";
    e4.size = 0;
    e4.date = "MAY 19 20:04";

    Entity entities[4];
    entities[0] = e1;
    entities[1] = e2;
    entities[2] = e3;
    entities[3] = e4;

    info_holder.entities_num = 4;
    info_holder.selected_line = 0;
    info_holder.entities = entities;

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