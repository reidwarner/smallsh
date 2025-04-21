#ifndef BUILTINS_H
#define BUILTINS_H

int exit_shell(int* bg_pids, int max);
int change_directory(char* path);
void print_status(int status);

#endif
