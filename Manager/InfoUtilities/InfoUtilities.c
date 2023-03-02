#include "InfoUtilities.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

static void on_error(const char *message, int *active) {
    perror(message);
    *active = -1;
    return;
}

enum DateOffsets {
    MONTH_SIZE = 3, MONTH_OFFSET = 4, DAY_SIZE = 2, DAY_OFFSET = 8, TIME_SIZE = 5, TIME_OFFSET = 11, YEAR_SIZE = 4, YEAR_OFFSET = 20
};

static void formate_and_write_date(char *dest, __time_t mod_time) {
    time_t seconds = time(NULL);
    char *src = ctime(&mod_time);
    int year_now = localtime(&seconds)->tm_year;
    int year_then  = localtime(&mod_time)->tm_year;
    snprintf(dest, MONTH_SIZE + 2, "%s ", src + MONTH_OFFSET);
    snprintf(dest + MONTH_SIZE + 1, DAY_SIZE + 2, "%s ", src + DAY_OFFSET);
    if (year_now == year_then) {
        snprintf(dest + MONTH_SIZE + DAY_SIZE + 2, TIME_SIZE + 1, "%s", src + TIME_OFFSET);
    } else {
        snprintf(dest + MONTH_SIZE + DAY_SIZE + 2, YEAR_SIZE + 1, "%s", src + YEAR_OFFSET);
    }
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
    formate_and_write_date(entity->date, file_info.st_mtim.tv_sec);
}