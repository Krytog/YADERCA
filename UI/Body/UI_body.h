#ifndef UI_body_H_INCLUDED
#define UI_body_H_INCLUDED

#include <ncurses.h>
#include "../../Manager/InfoHolder.h"

void init_UI_body(WINDOW **ptr, int terminal_width, int terminal_height);
void update_UI_body(WINDOW *ptr, int terminal_width, int terminal_height, const InfoHolder *info);
void destroy_UI_body(WINDOW *ptr);

#endif