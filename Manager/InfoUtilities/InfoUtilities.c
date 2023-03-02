#include "InfoUtilities.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include <fcntl.h>

static void on_error(const char *message, int *active) {
    perror(message);
    *active = -1;
    return;
}

void get_info(PathHolder *path, Entity *entity, struct dirent *info, int *active) {
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

    int fd = open("log.txt", O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);

    time(NULL);
    dprintf(fd, "%s\n", ctime(&file_info.st_mtim.tv_sec));

    close(fd);
}