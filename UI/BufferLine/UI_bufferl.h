#ifndef UI_bufferl_H_INCLUDED
#define UI_bufferl_H_INCLUDED

#include "../../Manager/InfoHolder/InfoHolder.h"
#include <ncurses.h>

void init_UI_bufferl(WINDOW **ptr, int terminal_width, int terminal_height);
void update_UI_bufferl(WINDOW *ptr, const InfoHolder *info_holder);
void destroy_UI_bufferl(WINDOW *ptr);

#endif