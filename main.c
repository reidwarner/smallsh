#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>

#include "parser.h"
#include "builtins.h"
#include "signals.h"
#include "executor.h"

#define NUM_CHILDREN 10

int main() {
    struct command_line* cmd;
    int childStatus = 0;
    pid_t lastFg = -1;
    int bg_child_pids[NUM_CHILDREN] = {0};

    // Setup signals
    setup_signal_handlers();

    while (true) {
        sigSTP = 0;  // reset SIGTSTP flag for each command

        // Prompt and parse user input
        printf(": ");
        fflush(stdout);
        cmd = parse_input();

        // Skip empty commands or comments
        if (cmd == NULL || cmd->argv[0] == NULL || cmd->argv[0][0] == '\0' || cmd->argv[0][0] == '#') {
            free_command(cmd);
            continue;
        }

        // Handle built-in commands
        if (strcmp(cmd->argv[0], "exit") == 0) {
            exit_shell(bg_child_pids, NUM_CHILDREN);
            free_command(cmd);
            break;
        } else if (strcmp(cmd->argv[0], "cd") == 0) {
            change_directory(cmd->argv[1]);
        } else if (strcmp(cmd->argv[0], "status") == 0) {
            print_status(childStatus);
        } else {
            // Execute non-built-in commands
            execute_command(cmd, bg_child_pids, &childStatus, &lastFg);
        }

        free_command(cmd);  // clean up memory
    }

    return EXIT_SUCCESS;
}
