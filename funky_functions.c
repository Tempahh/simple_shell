#include "shell.h"
/**
* signal_handler - handles signals for ctrl c
* @signal: signal number
* Return: nothing
*/
void signal_handler(int signal)
{
	(void)signal;
	write(1, "$ ", strlen("$ "));
	fflush(stdout);
}

/**
* get_user_input - displays a prompt and gets user input
* Return: returns the input
*/
char *get_user_input()
{
	char *input = NULL;

	size_t len = 0;
	ssize_t characters_read = 0;

	if (isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "$ ", 2);
		fflush(stdout);
	}

	characters_read = getline(&input, &len, stdin);

	if (characters_read == EOF)
	{
		free(input);
		return (NULL);
	}
	else if (characters_read == -1)
	{
		free(input);
		return (NULL);
	}

	if (input[characters_read - 1] == '\n')
	{
		input[characters_read - 1] = '\0';
	}

	return (input);
}

/**
* tokenize_input - breaks the users input into tokens
* @input: the user input to be tokenized
* Return: returns input
*/
char **tokenize_input(char *input)
{

	char **tokens = (char **)malloc(sizeof(char *) * 100);

	char *token;

	int i = 0;

	int tokenCount = 0;

	if (!tokens)
	{
		perror("malloc");
		return (NULL);
	}

	token = strtok(input, " ");

	while (token != NULL)
	{
		tokens[tokenCount] = strdup(token);
		if (!tokens[tokenCount])
		{
			perror("strdup");
			for (i = 0; i < tokenCount; i++)
			{

				free(tokens[i]);
			}
			free(tokens);
			return (NULL);
		}

		token = strtok(NULL, " ");
		tokenCount++;
	}

	tokens[tokenCount] = NULL;
	return (tokens);
}

/**
* is_path_command - checks if the command is an external command
* @command: the command to be checked
* Return: returns 0 at compeletion
*/
int is_path_command(char *command)
{

	int sl_i = 0;

	while (command[sl_i] != '\0')
	{
		if (command[sl_i] == '/')
		{
			return (1);
		}
		sl_i++;
	}
	return (0);
}
