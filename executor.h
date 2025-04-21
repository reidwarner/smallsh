#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

void execute_command(struct command_line* cmd, int* bg_pids, int* status, pid_t* lastFg);

#endif
