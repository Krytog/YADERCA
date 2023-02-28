#ifndef UI_legendl_H_INCLUDED
#define UI_legendl_H_INCLUDED

#include <ncurses.h>

void init_UI_legendl(WINDOW **ptr, int terminal_width);
void destroy_UI_legendl(WINDOW *ptr);

#endif