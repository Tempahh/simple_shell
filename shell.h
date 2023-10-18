#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>

extern char **environ;

void signal_handler(int signal);

ssize_t cus_getline(char **line, size_t *len, FILE *stream);

#endif
