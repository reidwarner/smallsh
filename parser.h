#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#define MAX_ARGS 512

struct command_line {
    char *argv[MAX_ARGS + 1];
    int argc;
    char *input_file;
    char *output_file;
    bool is_bg;
};

struct command_line* parse_input();
void free_command(struct command_line* cmd);

#endif