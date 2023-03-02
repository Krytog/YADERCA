#ifndef UI_bufferl_H_INCLUDED
#define UI_bufferl_H_INCLUDED

#include "../PathHolder/PathHolder.h"
#include "../InfoHolder/InfoHolder.h"

void get_info(PathHolder *path, Entity *entity, struct dirent *info, int *active);

#endif