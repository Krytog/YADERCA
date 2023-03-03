#include <unistd.h>
#include <sys/wait.h>

#define TERMINAL "gnome-terminal"

int open_text(const char *path) {
    pid_t pid = fork();
    if (pid < 0) {
        return 1;
    }
    else if (pid == 0) {
        execlp(TERMINAL, TERMINAL, "--", "nano", path, NULL);
    }
    else {
        int status;
        wait(&status);
    }
    return 0;
}
