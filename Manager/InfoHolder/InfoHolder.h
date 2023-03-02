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

typedef struct InfoHolder {
    int selected_line;
    size_t entities_num;
    Entity *entities;
} InfoHolder;

void info_holder_clean_up(InfoHolder *info_holder);

void info_holder_init(InfoHolder *info_holder);

#endif