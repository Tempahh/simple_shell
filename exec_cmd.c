#include "shell.h"
/**
 * *resolvePath - function to turn the path into an executable directory
 * @command: command passed from the keyboard
 * @pathdirs: unresolved path environment
 * Return: returns null at completion
*/

void exec_cmd(char *command, char *args[], char **pathdirs)
{

	pid_t child_pid;

	if (strchr(command, '/') != NULL)
	{
		child_pid = fork();

		if (child_pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (child_pid == 0)
		{
			execve(command, args, NULL);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		else
		{
			int status;

			waitpid(child_pid, &status, 0);

		}
	}
	else
	{
		char *fullpath = resolvePath(command, pathdirs);

		if (fullpath != NULL)
		{
			if (access(fullpath, X_OK) == 0)
			{
				child_pid = fork();

				if (child_pid == -1)
				{
					perror("fork");
					exit(EXIT_FAILURE);
				}
				else if (child_pid == 0)
				{
					execve(fullpath, args, NULL);
					perror("execve");
					exit(EXIT_FAILURE);
				}
				else
				{
					int status;

					waitpid(child_pid, &status, 0);
				}
				free(fullpath);
			}
			else
			{
				write(STDOUT_FILENO, "command not found\n", strlen("command not found\n"));
			}
		}
		else
		{
			write(STDOUT_FILENO, "command not found\n", strlen("command not found\n"));
		}
	}
}
