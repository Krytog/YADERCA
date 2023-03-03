#ifndef traveler_H_INCLUDED
#define traveler_H_INCLUDED

#include "../InfoHolder/InfoHolder.h"
#include "../PathHolder/PathHolder.h"

#include <dirent.h>

void go_into_dir(PathHolder *path, char *name, InfoHolder *info_holder, int *active);
void try_open_entity(PathHolder *current_path, InfoHolder *info_holder, int *active);

void switch_show_hidden(InfoHolder *info);

void delete_file(PathHolder *current_path, char *name, InfoHolder *infoHolder, int *active);
void add_to_buffer(PathHolder *current_path, char *name, InfoHolder *info_holder, int *active, int mode);
void clever_insert_from_buffer(PathHolder *current_path, InfoHolder *info_holder, int *active);

#endif