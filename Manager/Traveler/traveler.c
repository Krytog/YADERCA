#include "traveler.h"
#include "../InfoUtilities/InfoUtilities.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


static void on_error(const char *message, int *active) {
    perror(message);
    *active = -1;
    return;
}

static void update_current_path(PathHolder *path, char *name, int *active) {
    size_t name_size = strlen(name);
    if (path->end_pos + name_size >= PATH_MAX) {
        on_error("update_current_path: exceed path max", active);
    }
    snprintf(path->current_path + path->end_pos, PATH_MAX + 1, "%s", name);
    path->end_pos += name_size;
    snprintf(path->current_path + path->end_pos, 2, "/");
    ++path->end_pos;
}

static void formate_and_write_date(char *dest, char *src) {
    snprintf(dest, 3, "%s", src + 4);
}

static void clean_up(struct dirent **ent, int size) {
    for (int i = 0; i < size; ++i) {
        free(ent[i]);
    }
    free(ent);
}

void go_into_dir(PathHolder *path, char *name, InfoHolder *info_holder, int *active) {
    update_current_path(path, name, active);
    struct dirent **ent;
    int size = scandir(path->current_path, &ent, NULL, alphasort);
    if (size < 0) {
        perror("scandir failed, path is\n");
        perror(path->current_path);
        *active = -1;
        return;
    }
    info_holder_clean_up(info_holder);
    info_holder->entities = calloc(size - 1, sizeof(*info_holder->entities));
    for (int i = 1; i < size; ++i) {
        get_info(path, info_holder->entities + i - 1, ent[i], active);
    }
    info_holder->entities_num = size - 1;
    clean_up(ent, size);
}