#ifndef traveler_H_INCLUDED
#define traveler_H_INCLUDED

#include "../InfoHolder/InfoHolder.h"

#include <dirent.h>

typedef struct PathHolder {
    char current_path[PATH_MAX];
    size_t end_pos;
} PathHolder;

void path_holder_init(PathHolder *path_holder);

void go_into_dir(PathHolder *current_path, char *name, InfoHolder *info_holder, int *active);

#endif