#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include "builtins.h"

int exit_shell(int* bg_pids, int max) {
    for (int i = 0; i < max; ++i) {
        if (bg_pids[i] > 0) {
            kill(bg_pids[i], SIGTERM);
            waitpid(bg_pids[i], NULL, 0);
        }
    }
    exit(0);
}

int change_directory(char* path) {
    const char* home = getenv("HOME");
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    if (path == NULL) {
        chdir(home);
    } else if (path[0] == '/') {
        chdir(path);
    } else {
        size_t new_path_len = strlen(cwd) + strlen(path) + 2;
        char* new_path = malloc(new_path_len);
        if (new_path != NULL) {
            snprintf(new_path, new_path_len, "%s/%s", cwd, path);
            chdir(new_path);
            free(new_path);
        }
    }
    return 0;
}

void print_status(int status) {
    if (WIFEXITED(status)) {
        printf("exit value %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("terminated by signal %d\n", WTERMSIG(status));
    }
    fflush(stdout);
}
