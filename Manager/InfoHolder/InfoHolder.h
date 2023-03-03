#ifndef InfoHolder_H_INCLUDED
#define InfoHolder_H_INCLUDED

#include <stdlib.h>

enum {
    DATE_SIZE = 13
};

typedef struct Entity {
    unsigned char type;
    char *name;
    size_t size;
    char *date;
} Entity;

typedef struct NotHiddenInfo {
    size_t *indices;
    size_t num;
} NotHiddenInfo;

typedef struct InfoHolder {
    int selected_line;
    size_t entities_num;
    Entity *entities;
    NotHiddenInfo not_hidden_info;
    int show_hidden;
} InfoHolder;

void info_holder_clean_up(InfoHolder *info_holder);

void info_holder_init(InfoHolder *info_holder);

void decrease_selected_line(InfoHolder *info_holder);
void increase_selected_line(InfoHolder *info_holder);

char *get_selected_name(InfoHolder *info_holder);
size_t get_upper_limit(InfoHolder *info_holder);

#endif