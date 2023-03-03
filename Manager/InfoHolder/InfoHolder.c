#include "InfoHolder.h"

void info_holder_clean_up(InfoHolder *info_holder) {
    for (int i = 0; i < info_holder->entities_num; ++i) {
        free(info_holder->entities[i].name);
        free(info_holder->entities[i].date);
    }
    free(info_holder->entities);
    free(info_holder->not_hidden_info.indices);
}

void info_holder_init(InfoHolder *info_holder) {
    info_holder->entities_num = 0;
    info_holder->entities = NULL;
    info_holder->selected_line = 0;
    info_holder->show_hidden = 0;
    info_holder->not_hidden_info.indices = NULL;
    info_holder->not_hidden_info.indices = 0;
}

void decrease_selected_line(InfoHolder *info_holder) {
    if (info_holder->selected_line > 0) {
        --info_holder->selected_line;
    }
}

void increase_selected_line(InfoHolder *info_holder) {
    if (info_holder->selected_line + 1 < get_upper_limit(info_holder)) {
        ++info_holder->selected_line;
    }
}

char *get_selected_name(InfoHolder *info_holder) {
    if (info_holder->show_hidden) {
        return info_holder->entities[info_holder->selected_line].name;
    }
    return info_holder->entities[info_holder->not_hidden_info.indices[info_holder->selected_line]].name;
}

size_t get_upper_limit(InfoHolder *info_holder) {
    if (info_holder->show_hidden) {
        return info_holder->entities_num;
    }
    return info_holder->not_hidden_info.num;
}