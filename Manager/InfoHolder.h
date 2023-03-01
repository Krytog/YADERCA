#ifndef InfoHolder_H_INCLUDED
#define InfoHolder_H_INCLUDED

#include <stdlib.h>

typedef struct InfoHolder {
    int selected_line;
    size_t entities_num;
    char **names;
    char **sizes;
    char **dates;
} InfoHolder;

#endif