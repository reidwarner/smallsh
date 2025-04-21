#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define INPUT_LENGTH 2048

struct command_line* parse_input() {
    char input[INPUT_LENGTH];
    struct command_line* cmd = calloc(1, sizeof(struct command_line));

    printf(": ");
    fflush(stdout);
    fgets(input, INPUT_LENGTH, stdin);

    if (strlen(input) == 1) {
        cmd->argv[0] = "";
        return cmd;
    }

    char* token = strtok(input, " \n");
    while (token) {
        if (!strcmp(token, "<")) {
            cmd->input_file = strdup(strtok(NULL, " \n"));
        } else if (!strcmp(token, ">")) {
            cmd->output_file = strdup(strtok(NULL, " \n"));
        } else if (!strcmp(token, "&")) {
            cmd->is_bg = true;
        } else {
            cmd->argv[cmd->argc++] = strdup(token);
        }
        token = strtok(NULL, " \n");
    }
    return cmd;
}

void free_command(struct command_line* cmd) {
    for (int i = 0; i < cmd->argc; ++i) free(cmd->argv[i]);
    free(cmd->input_file);
    free(cmd->output_file);
    free(cmd);
}
