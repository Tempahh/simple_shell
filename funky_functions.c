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
/*
* cus_getline- custom getline function
* @line: line buffer
* @len: length of buffer
* @stream: stream to read from
* Return: number of characters read or -1 if fail
*/
ssize_t cus_getline(char **line, size_t *len, FILE *stream)
{
	char *readline;

	ssize_t read;

	if (*line == NULL || *len == 0)
	{
		*len = 1024;
		*line = malloc(*len);
		if (*line == NULL)
		{
			return (-1);
		}
	}

	readline = fgets(*line, *len, stream);
	if (readline != NULL)
	{
		read = strlen(*line);

		if (read > 0 && (*line)[read - 1] == '\n')
		{
			(*line)[read - 1] = '\0';
			read--;
		}
		return (read);
	}
	else
	{
		return (-1);
	}
}
