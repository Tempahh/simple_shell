#include "shell.h"
/**
 * *resolvePath - function to turn the path into an executable directory
 * @command: command passed from the keyboard
 * @pathdirs: unresolved path environment
 * Return: returns null at completion
*/
char *resolvePath(char *command, char **pathdirs)
{
	char *fullpath = NULL;

	int i = 0;

	for (i = 0; pathdirs[i] != NULL; i++)
	{
		fullpath = (char *)malloc(strlen(pathdirs[i]) + strlen(command) + 2);

		if (fullpath == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		sprintf(fullpath, "%s/%s", pathdirs[i], command);

		if (access(fullpath, X_OK) == 0)
		{
			return (fullpath);
		}

		free(fullpath);
	}
	free(fullpath);
	return (NULL);
}
