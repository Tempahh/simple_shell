#include "shell.h"

ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream)
{

	ssize_t chars_read = 0;
	size_t bufsize = *n;
	size_t i = 0;
	int c;

	(void)chars_read;

	if (!lineptr || !n || !stream)
	{
		return (-1);
	}

	if (*lineptr == NULL)
	{
		*lineptr = (char *)malloc(INITIAL_BUFFER_SIZE);
		if (*lineptr == NULL)
		{
			return (-1);
		}
		bufsize = INITIAL_BUFFER_SIZE;
	}

	while (1)
	{
		c = fgetc(stream);

		if (c == EOF || c == '\n')
		{
			if (c == EOF && i == 0)
			{
				return (-1);
			}
			(*lineptr)[i] = '\0';
			break;
		}

		(*lineptr)[i] = (char)c;
		i++;

		if (i >= bufsize - 1)
		{
			bufsize *= 2;
			*lineptr = (char *)realloc(*lineptr, bufsize);
			if (*lineptr == NULL)
			{
				return (-1);
			}
		}
	}
	*n = bufsize;
	return (i);
}

char *custom_strtok(char *string, const char *delim)
{
	static char *current;
	char *start;

	current = NULL;

	if (string != NULL)
	{
		current = string;
	}
	else if (current == NULL || *current == '\0')
	{
		return (NULL);
	}

	while (*current != '\0' && strchr(delim, *current) != NULL)
	{
		current++;
	}

	if (*current == '\0')
	{
		return (NULL);
	}

	start = current;

	while (*current != '\0' && strchr(delim, *current) == NULL)
	{
		current++;
	}

	if (*current != '\0')
	{
		*current = '\0';
		current++;
	}

	return (start);
}

