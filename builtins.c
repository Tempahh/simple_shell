#include "shell.h"
/**
 * exit_bin - function to handle exiting the shell builtins
 * @args: possible arguments
*/
void exit_bin(char **args)
{
	int status = 0;

	if (args[1] != NULL)
	{
		status = atoi(args[1]);
		exit(status);
	}
	else
	{
		exit(0);
	}
}
/**
 * print_env - function to hold and print the shell environment
 * @args: possible arguments
*/
void print_env(char **args)
{
	char **env = environ, *envVar, *equalsSign;

	size_t keyLength;

	(void)args;

	while (*env != NULL)
	{
		envVar = *env;

		equalsSign = strchr(envVar, '=');

		if (equalsSign != NULL)
		{
			keyLength = equalsSign - envVar;

			write(STDOUT_FILENO, envVar, keyLength);
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, equalsSign + 1, strlen(equalsSign + 1));
			write(STDOUT_FILENO, "\n", 1);
		}

		env++;
	}
}
/**
 * my_setenv - function to set environment
 * @args: possible trailing arguments
*/
void my_setenv(char **args)
{
	if (args[1] == NULL || args[2] == NULL || args[3] != NULL)
	{
		fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		return;
	}
	if (setenv(args[1], args[2], 1) != 0)
	{
		perror("setenv");
	}
}
/**
 * my_unsetenv - function to unset environment
 * @args: possible trailing arguments
*/
void my_unsetenv(char **args)
{
	if (args[1] == NULL || args[2] != NULL)
	{
		fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		return;
	}
	if (unsetenv(args[1]) != 0)
	{
		perror("unsetenv");
	}
}
/**
 * change_dir - function to handle changing directory
 * @args: possible trailing arguments
*/
void change_dir(char **args)
{
	char *home = getenv("HOME");

	char *old_dir = getenv("OLDPWD");

	if (args[1] == NULL)
	{
		chdir(home);
	}
	else if (strcmp(args[1], "-") == 0)
	{
		chdir(old_dir);
	}
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("chdir");
		}
		else
		{
		setenv("OLDPWD", getcwd(NULL, 0), 1);
		setenv("PWD", args[1], 1);
		}
	}
}
