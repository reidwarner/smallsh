#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "builtins.h"
#include "signals.h"

#define NUM_CHILDREN 10

int bg_child_pids[NUM_CHILDREN] = {0};
int childStatus = 0;
pid_t lastFg;

int main() {
    setup_signal_handlers();

    while (true) {
        sigSTP = 0;
        struct command_line* cmd = parse_input();

        if (cmd == NULL || cmd->argv[0] == NULL || strcmp(cmd->argv[0], "") == 0 || cmd->argv[0][0] == '#') {
            free_command(cmd);
            continue;
        }

        if (strcmp(cmd->argv[0], "exit") == 0) {
            free_command(cmd);
            exit_shell(bg_child_pids, NUM_CHILDREN);
        } else if (strcmp(cmd->argv[0], "cd") == 0) {
            change_directory(cmd->argv[1]);
        } else if (strcmp(cmd->argv[0], "status") == 0) {
            print_status(childStatus);
        } else {
            // fork and exec logic here (or call executor module)
        }

        free_command(cmd);
    }

    return 0;
}
