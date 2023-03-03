#ifndef traveler_H_INCLUDED
#define traveler_H_INCLUDED

#include "../InfoHolder/InfoHolder.h"
#include "../PathHolder/PathHolder.h"

#include <dirent.h>

void go_into_dir(PathHolder *current_path, char *name, InfoHolder *info_holder, int *active);

void delete_file(PathHolder *current_path, char *name, InfoHolder *infoHolder, int *active);

void switch_show_hidden(InfoHolder *info);

#endif