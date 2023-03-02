#ifndef input_handler_H_INCLUDED
#define input_handler_H_INCLUDED

#include "../InfoHolder/InfoHolder.h"
#include "../PathHolder/PathHolder.h"

void wait_for_and_handle_input(PathHolder *path_holder, InfoHolder *info, int *active);

#endif