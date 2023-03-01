#ifndef InfoHolder_H_INCLUDED
#define InfoHolder_H_INCLUDED

#include <stdlib.h>

typedef struct Entity {
    int type;
    const char *name;
    size_t size;
    const char *date;
} Entity;

typedef struct InfoHolder {
    int selected_line;
    size_t entities_num;
    Entity *entities;
} InfoHolder;

#endif