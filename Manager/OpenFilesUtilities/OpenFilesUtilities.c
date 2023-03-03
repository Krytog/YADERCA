#include "OpenFilesUtilities.h"

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define CONFIG_PATH "../OpenFiles/CONFIG.txt"

typedef struct FuncInfo {
    char func_name[PATH_MAX];
    char library_name[PATH_MAX];
} FuncInfo;


static int raw_equal(const char *first, const char *second, size_t ffrom, size_t sfrom, size_t size) {
    for (int i = 0; i < size; ++i) {
        if (first[ffrom + i] != second[sfrom + i]) {
            return 0;
        }
    }
    return 1;
}

static int get_func_info(const char *path, FuncInfo *func_info) {
    size_t end = strlen(path);
    if (end == 0) {
        return 1;
    }
    size_t begin = end - 1;
    while (begin > 0 && path[begin] != '.') {
        --begin;
    }
    size_t size = end - begin;

    FILE *config = fopen(CONFIG_PATH, "r");
    if (!config) {
        return 1;
    }
    char *line = calloc(3 * PATH_MAX, sizeof(*line));
    if (!line) {
        fclose(config);
        return 1;
    }

    int out = 1;
    while (fgets(line, 3 * PATH_MAX, config)) {
        line[strcspn(line, "\n")] = '\0';
        if (!raw_equal(path, line, begin, 0, size)) {
            continue;
        }
        size_t ptr = size + 1;
        while (line[ptr] != ' ') {
            func_info->func_name[ptr - size - 1] = line[ptr];
            ++ptr;
        }
        func_info->func_name[ptr - size + 1] = '\0';
        snprintf(func_info->library_name, PATH_MAX, "%s", line + ptr + 1);
        out = 0;
        break;
    }

    free(line);
    fclose(config);
    return out;
}

void open_file(const char *path) {
    FuncInfo func_info;
    if (get_func_info(path, &func_info)) {
        return;
    }
    void *lib = dlopen(func_info.library_name, RTLD_NOW);
    int (*func)(const char *) = dlsym(lib, func_info.func_name);
    func(path);
    dlclose(lib);
}