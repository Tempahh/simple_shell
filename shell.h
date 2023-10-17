#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

extern char **environ;

typedef struct
{
	char *name;

	void (*func)(char **args);

} BuiltInCommand;

#define INITIAL_BUFFER_SIZE 128

void exec_cmd(char *command, char *args[], char **pathdirs);

char *resolvePath(char *command, char **pathdirs);

void exit_bin(char **args);

void print_env(char **args);

ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream);

char *custom_strtok(char *string, const char *delim);

void my_setenv(char **args);

void my_unsetenv(char **args);

void change_dir(char **args);

extern BuiltInCommand builtins[];

#endif
