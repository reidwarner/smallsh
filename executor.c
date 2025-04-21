#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include "executor.h"
#include "signals.h"

#define NUM_CHILDREN 10

void add_bg_pid(pid_t pid, int* bg_pids) {
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (bg_pids[i] == 0) {
            bg_pids[i] = pid;
            break;
        }
    }
}

void check_bg_processes(int* bg_pids) {
    int status;
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (bg_pids[i] > 0) {
            pid_t result = waitpid(bg_pids[i], &status, WNOHANG);
            if (result > 0) {
                if (WIFEXITED(status)) {
                    printf("background pid %d is done: exit value %d\n", bg_pids[i], WEXITSTATUS(status));
                } else if (WIFSIGNALED(status)) {
                    printf("background pid %d is done: terminated by signal %d\n", bg_pids[i], WTERMSIG(status));
                }
                fflush(stdout);
                bg_pids[i] = 0;
            }
        }
    }
}

void setup_redirection(struct command_line* cmd) {
    int fd;
    if (cmd->input_file) {
        fd = open(cmd->input_file, O_RDONLY);
        if (fd == -1 || dup2(fd, STDIN_FILENO) == -1) {
            printf("cannot open %s for input\n", cmd->input_file);
            fflush(stdout);
            exit(1);
        }
    }

    if (cmd->output_file) {
        fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1) {
            printf("cannot open %s for output\n", cmd->output_file);
            fflush(stdout);
            exit(1);
        }
    }

    // BG process w/ no I/O redirection
    if (cmd->is_bg && fgMode == 0) {
        if (!cmd->input_file) {
            fd = open("/dev/null", O_RDONLY);
            dup2(fd, STDIN_FILENO);
        }
        if (!cmd->output_file) {
            fd = open("/dev/null", O_WRONLY);
            dup2(fd, STDOUT_FILENO);
        }
    }
}

void execute_command(struct command_line* cmd, int* bg_pids, int* status, pid_t* lastFg) {
    pid_t spawnPid = fork();

    switch (spawnPid) {
        case -1:
            perror("fork");
            exit(1);
            break;

        case 0: {
            // CHILD process
            if (cmd->is_bg && fgMode == 0) {
                // Background: ignore SIGINT
                struct sigaction ignore_action = {0};
                ignore_action.sa_handler = SIG_IGN;
                sigaction(SIGINT, &ignore_action, NULL);
            } else {
                // Foreground: default SIGINT
                struct sigaction default_action = {0};
                default_action.sa_handler = SIG_DFL;
                sigaction(SIGINT, &default_action, NULL);
            }

            // All children ignore SIGTSTP
            struct sigaction ignore_tstp = {0};
            ignore_tstp.sa_handler = SIG_IGN;
            sigaction(SIGTSTP, &ignore_tstp, NULL);

            // Avoid re-running if SIGTSTP just toggled
            if (sigSTP == 1) exit(0);

            setup_redirection(cmd);

            execvp(cmd->argv[0], cmd->argv);
            printf("%s: no such file or directory\n", cmd->argv[0]);
            fflush(stdout);
            exit(1);
            break;
        }

        default:
            // PARENT process
            if (cmd->is_bg && fgMode == 0) {
                printf("background pid is %d\n", spawnPid);
                fflush(stdout);
                add_bg_pid(spawnPid, bg_pids);
            } else {
                *lastFg = spawnPid;
                waitpid(spawnPid, status, 0);

                if (WIFSIGNALED(*status)) {
                    printf("terminated by signal %d\n", WTERMSIG(*status));
                    fflush(stdout);
                }
            }

            check_bg_processes(bg_pids);
            break;
    }
}
