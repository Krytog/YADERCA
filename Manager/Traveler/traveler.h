#ifndef traveler_H_INCLUDED
#define traveler_H_INCLUDED

#include "../InfoHolder/InfoHolder.h"
#include "../PathHolder/PathHolder.h"

#include <dirent.h>

void go_into_dir(PathHolder *current_path, char *name, InfoHolder *info_holder, int *active);

#endif