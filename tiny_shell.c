#include "shell.h"
/**
 * main - Entry point of the shell
 * return: return 0 on completion
*/
int main()
{
	char *myCommand = NULL;

	size_t bufferSize = 0;
	ssize_t charsRead = 0;
	char *token, **args = NULL;
	char *fullpath;

	int i, argCount;

	char *path = getenv("PATH");

	char **pathdir = (char **)malloc(sizeof(char *) * 100);

	int path_count = 0;

	char *pathToken = strtok(path, ":");


	BuiltInCommand builtins[] = {
		{"exit", exit_bin},
		{"env", print_env},
		{"setenv", my_setenv},
		{"unsetenv", my_unsetenv},
		{"cd", change_dir},
		{NULL, NULL}
	};

	while (pathToken != NULL)
	{
		pathdir[path_count] = pathToken;
		path_count++;
		pathToken = strtok(NULL, ":");
	}
	pathdir[path_count] = NULL;

	while (1)
	{
		write(STDOUT_FILENO, "$ ", 2);

		charsRead = custom_getline(&myCommand, &bufferSize, stdin);
		if (charsRead == -1)
		{
			perror("getline");
			exit(EXIT_FAILURE);
		}
		if (myCommand[charsRead - 1] == '\n')
		{
			myCommand[charsRead - 1] = '\0';
		}

		args = (char **)malloc(sizeof(char *) * 100);
		argCount = 0;

		token = strtok(myCommand, " ");
		while (token != NULL)
		{
			args[argCount] = strdup(token);
			if (args[argCount] == NULL)
			{
				perror("strdup");
				exit(EXIT_FAILURE);
			}
			argCount++;
			token = strtok(NULL, " ");
		}
		args[argCount] = NULL;

		fullpath = resolvePath(args[0], pathdir);

		for (i = 0; builtins[i].name != NULL; i++)
		{
			if (strcmp(args[0], builtins[i].name) == 0)
			{
				builtins[i].func(args);
				break;
			}
		}

		if (builtins[i].name == NULL)
		{
			exec_cmd(args[0], args, pathdir);
		}

		if (fullpath != NULL)
		{
			printf("Resolved path: %s\n", fullpath);
		}

		for (i = 0; i < argCount; i++)
		{
			free(args[i]);
		}
		free(args);
	}

	free(myCommand);
	free(pathdir);

	return (0);
}
