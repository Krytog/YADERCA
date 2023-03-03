#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include "../Manager/InfoHolder/InfoHolder.h"

enum Colors {
    DEFAULT_COLOR = 4
};

void init_UI();
void draw_UI(const InfoHolder *info);
void destroy_UI();

#endif