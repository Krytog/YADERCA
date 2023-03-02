#ifndef PathHolder_H_INCLUDED
#define PathHolder_H_INCLUDED

#include <dirent.h>

typedef struct PathHolder {
    char current_path[PATH_MAX];
    size_t end_pos;
} PathHolder;

void path_holder_init(PathHolder *path_holder);

#endif