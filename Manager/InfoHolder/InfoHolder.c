#include "InfoHolder.h"

void info_holder_clean_up(InfoHolder *info_holder) {
    for (int i = 0; i < info_holder->entities_num; ++i) {
        free(info_holder->entities[i].name);
        free(info_holder->entities[i].date);
    }
    free(info_holder->entities);
}

void info_holder_init(InfoHolder *info_holder) {
    info_holder->entities_num = 0;
    info_holder->entities = NULL;
    info_holder->selected_line = 0;
}