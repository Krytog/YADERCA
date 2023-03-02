#include "traveler.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void path_holder_init(PathHolder *path_holder) {
    path_holder->end_pos = 0;
}

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

static void get_info(PathHolder *path, Entity *entity, struct dirent *info, int *active) {
    struct stat file_info;
    char buffer[PATH_MAX];
    snprintf(buffer, path->end_pos + 1, "%s", path->current_path);
    snprintf(buffer + path->end_pos, strlen(info->d_name) + 1, "%s", info->d_name);
    lstat(buffer, &file_info);
    int name_size = strlen(info->d_name);
    entity->type = info->d_type;
    entity->name = calloc(name_size + 1, sizeof(*entity->name));
    if (!entity->name) {
        on_error("get_info: calloc for name failed", active);
    }
    snprintf(entity->name, name_size + 1, "%s", info->d_name);
    entity->size = file_info.st_size;
    entity->date = calloc(DATE_SIZE + 1, sizeof(*entity->date));
    if (!entity->date) {
        on_error("get_info: calloc for date failed", active);
    }
    snprintf(entity->date, DATE_SIZE + 1, "%ld", file_info.st_mtim.tv_sec);
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
    info_holder->entities = calloc(size, sizeof(*info_holder->entities));
    for (int i = 0; i < size; ++i) {
        get_info(path, info_holder->entities + i, ent[i], active);
    }
    info_holder->entities_num = size;
    clean_up(ent, size);
}