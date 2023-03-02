#include "traveler.h"
#include "../InfoUtilities/InfoUtilities.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BACK ".."

static void on_error(const char *message, int *active) {
    perror(message);
    *active = -1;
    return;
}

static void delete_last_entry_from_path(PathHolder *path) {
    if (path->end_pos > 1) {
        path->end_pos -= 2;
    }
    while (path->end_pos > 0 && path->current_path[path->end_pos] != '/') {
        --path->end_pos;
    }
    path->current_path[++path->end_pos] = '\0';
}

static void update_current_path(PathHolder *path, char *name, int *active) {
    if (!strcmp(name, BACK)) {
        delete_last_entry_from_path(path);
        return;
    }
    size_t name_size = strlen(name);
    if (path->end_pos + name_size >= PATH_MAX) {
        on_error("update_current_path: exceed path max", active);
    }
    snprintf(path->current_path + path->end_pos, PATH_MAX + 1, "%s", name);
    path->end_pos += name_size;
    snprintf(path->current_path + path->end_pos, 2, "/");
    ++path->end_pos;
}

static void clean_up(struct dirent **ent, int size) {
    for (int i = 0; i < size; ++i) {
        free(ent[i]);
    }
    free(ent);
}

static void init_start_path(PathHolder *path) {
    getcwd(path->current_path, PATH_MAX);
    path->end_pos += strlen(path->current_path);
    path->current_path[path->end_pos] = '/';
    ++path->end_pos;
}

static void update_info(PathHolder *path, InfoHolder *info_holder, int *active) {
    struct dirent **ent;
    int size = scandir(path->current_path, &ent, NULL, alphasort);
    if (size <= 0) {
        delete_last_entry_from_path(path);
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

void go_into_dir(PathHolder *path, char *name, InfoHolder *info_holder, int *active) {
    if (name == NULL) {
        init_start_path(path);
    } else {
        update_current_path(path, name, active);
    }
    update_info(path, info_holder, active);
    info_holder->selected_line = 0;
}

void delete_file(PathHolder *path, char *name, InfoHolder *info_holder, int *active) {
    if (!strcmp(name, BACK)) {
        return;
    }
    update_current_path(path, name, active);
    path->current_path[--path->end_pos] = '\0';
    remove(path->current_path);
    path->current_path[path->end_pos++] = '/';
    delete_last_entry_from_path(path);
    update_info(path, info_holder, active);
    if (info_holder->selected_line == info_holder->entities_num) {
        --info_holder->selected_line;
    }
}
