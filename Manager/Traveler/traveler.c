#include "traveler.h"
#include "../InfoUtilities/InfoUtilities.h"
#include "../OpenFilesUtilities/OpenFilesUtilities.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BACK ".."
#define HIDDEN_SIGN '.'

#define COPY_BUFFER_SIZE 4096

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
    info_holder->not_hidden_info.indices = calloc(size - 1, sizeof(*info_holder->not_hidden_info.indices));
    size_t not_hidden_num = 0;
    for (int i = 1; i < size; ++i) {
        get_info(path, info_holder->entities + i - 1, ent[i], active);
        if (ent[i]->d_name[0] != HIDDEN_SIGN || !strcmp(ent[i]->d_name, BACK)) {
            info_holder->not_hidden_info.indices[not_hidden_num++] = i - 1;
        }
    }
    info_holder->not_hidden_info.num = not_hidden_num;
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
    if (info_holder->selected_line == get_upper_limit(info_holder)) {
        --info_holder->selected_line;
    }
}

void switch_show_hidden(InfoHolder *info_holder) {
    if (info_holder->show_hidden) {
        info_holder->show_hidden = 0;
    } else {
        info_holder->show_hidden = 1;
    }
    info_holder->selected_line = 0;
}

static int copy_file_from_buffer(PathHolder *current_path, InfoHolder *info_holder, int *active) {
    update_current_path(current_path, info_holder->buffer.name, active);
    current_path->current_path[--current_path->end_pos] = '\0';
    int dist = open(current_path->current_path, O_RDONLY);
    if (dist != -1) {
        close(dist);
        delete_last_entry_from_path(current_path);
        return EXIT_FAILURE;
    }
    int src = open(info_holder->buffer.path, O_RDONLY);
    if (src == -1) {
        delete_last_entry_from_path(current_path);
        return EXIT_FAILURE;
    }
    dist = open(current_path->current_path, O_CREAT | O_WRONLY);
    if (dist == -1) {
        close(src);
        delete_last_entry_from_path(current_path);
        return EXIT_FAILURE;
    }
    char copy_buffer[COPY_BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(src, &copy_buffer, COPY_BUFFER_SIZE)) > 0) {
        ssize_t bytes_written;
        char *offset_buffer = copy_buffer;
        do {
            bytes_written = write(dist, offset_buffer, bytes_read);
            if (bytes_written >= 0) {
                bytes_read -= bytes_written;
                offset_buffer += bytes_written;
            }
        } while (bytes_read > 0);
    }
    close(src);
    close(dist);
    struct stat st;
    stat(info_holder->buffer.path, &st);
    chmod(current_path->current_path, st.st_mode);
    delete_last_entry_from_path(current_path);
    return EXIT_SUCCESS;
}

void add_to_buffer(PathHolder *current_path, char *name, InfoHolder *info_holder, int *active, int mode) {
    snprintf(info_holder->buffer.name, NAME_MAX, "%s", name);
    update_current_path(current_path, name, active);
    snprintf(info_holder->buffer.path, PATH_MAX, "%s", current_path->current_path);
    info_holder->buffer.path[current_path->end_pos - 1] = '\0';
    delete_last_entry_from_path(current_path);
    info_holder->buffer.mode = mode;
}

void clever_insert_from_buffer(PathHolder *current_path, InfoHolder *info_holder, int *active) {
    switch (info_holder->buffer.mode) {
        case BUFFER_MODE_EMPTY: {
            return;
        }
        case BUFFER_MODE_COPY: {
            copy_file_from_buffer(current_path, info_holder, active);
            update_info(current_path, info_holder, active);
            break;
        }
        case BUFFER_MODE_CUT: {
            if (copy_file_from_buffer(current_path, info_holder, active)) {
                return;
            }
            remove(info_holder->buffer.path);
            info_holder_buffer_clear(info_holder);
            update_info(current_path, info_holder, active);
            break;
        }
    }
}

void go_into_file(PathHolder *current_path, char *name, int *active) {
    update_current_path(current_path, name, active);
    current_path->current_path[--current_path->end_pos] = '\0';
    open_file(current_path->current_path);
    delete_last_entry_from_path(current_path);
}

void try_open_entity(PathHolder *current_path, InfoHolder *info_holder, int *active) {
    if (get_selected_type(info_holder) == DT_DIR) {
        go_into_dir(current_path, get_selected_name(info_holder), info_holder, active);
    } else {
        go_into_file(current_path, get_selected_name(info_holder), active);
    }
}
