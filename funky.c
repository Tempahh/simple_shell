#include "shell.h"


/**
 * main - creating a funky shell replica
 * @ac: argument count
 * @av: argument vector
 *
 * description: program does the following
 * 1. prompts the user for a command
 * 2. the command is then passed into the program through main ()
 * 3. the command gets tokenized
 * 4. if the initial command contains a '/' then it is a directory
 * 5. if it does not, we treat it as a command
 * 6. if we find the exec file in a path then we execute it
 * 7. if we can't find it in the path then we handle it as a builtin
 * 8. that has the same name as the program using strcmp. if strcmp == 0,
 * execve(strcat(directory, line), arguments, NULL);
 * 4. trailing strings will now be treated as arguments to the command
 * 5. forks program to create a new process (child)
 * 6. uses execve function to execute the cmd in the child process
 * 7. parent process waits for another command to be inputed by user
 *
 * Return: always 0
 */

int main(int ac, char **av)
{
	ssize_t characters_read = 0;

	ssize_t prompt_bytes_written = 0;

	char *line = NULL;
	size_t len = 0;
	size_t i = 0;

	size_t free_dir_index = 0;

	const char *delimiters = NULL;
	char *command_token = NULL;

	char **cmd = NULL;

	size_t max_command_allowed = 0;

	int running_pid = 0;
	int status = 0;

	char *path_environment_variable = NULL;

	int sl_i = 0;
	char *path = NULL;

	size_t max_dir = 0;

	char **dirs = NULL;
	char *dir_token = NULL;
	size_t directory_index = 0;

	DIR *dir_stream = NULL;
	size_t search_index = 0;

	struct dirent *entry = NULL;

	char *concatenated_path = NULL;

	int found = 0;

	char *err_msg = NULL;

	size_t size = 0;

	(void)ac;


	signal(SIGINT, signal_handler);

	while (1)
	{
		line = NULL;
		delimiters = " \n\t\r\a";
		path = NULL;

		if (isatty(STDIN_FILENO))
		{
			prompt_bytes_written = write(1, "$ ", strlen("$ "));
			if (prompt_bytes_written == -1)
			{
				perror("write");
				exit(1);
			}
		}

		fflush(stdout);


		characters_read = cus_getline(&line, &len, stdin);

		if (characters_read == EOF)
		{
			break;
		}
		else if (characters_read == -1)
		{
			break;
		}

		if (strcmp(line, "exit") == 0)
		{
			free(line);
			exit(0);
		}

		if (strcmp(line, "env") == 0)
		{
			int i = 0;

			for (; environ[i]; i++)
			{
				write(1, environ[i], strlen(environ[i]));
				write(1, "\n", 1);
			}
			free(line);
			continue;
		}

		command_token = strtok(line, delimiters);
		if (command_token == NULL)
		{
			free(line);
			continue;
		}

		max_command_allowed = 10;
		cmd = malloc(sizeof(char *) * max_command_allowed);
		i = 0;
		while (command_token != NULL)
		{
			if (i >= max_command_allowed)
			{
				max_command_allowed *= 2;
				cmd = realloc(cmd, sizeof(char *) * max_command_allowed);
				if (cmd == NULL)
				{
					perror("./hsh");
				}
			}

			cmd[i] = strdup(command_token);
			if (cmd[i] == NULL)
				continue;
			command_token = strtok(NULL, delimiters);
			i++;
		}

		cmd[i] = NULL;

		sl_i = 0;
		while (line[sl_i] != '\0')
		{
			if (line[sl_i] == '/')
			{
				path = line;
				break;
			}
			sl_i++;
		}


		if (path == NULL)
		{

			char *PATH = getenv("PATH");

			if (PATH == NULL)
				continue;
			path_environment_variable = strdup(PATH);
			if (path_environment_variable == NULL)
				continue;

			dir_token = strtok(path_environment_variable, ":");
			if (dir_token == NULL)
			{
				perror("av[0]");
			}


			max_dir = 100;
			dirs = malloc(sizeof(char *) * max_dir);
			directory_index = 0;

			while (dir_token != NULL)
			{
				if (directory_index >= max_dir)
				{
					max_dir *= 2;
					dirs = realloc(dirs, sizeof(char *) * max_dir);
					if (dirs == NULL)
					{
						perror("realloc");
					}
				}
				dirs[directory_index] = strdup(dir_token);
				if (PATH == NULL)
					continue;
				dir_token = strtok(NULL, ":");
				directory_index++;
			}

			dirs[directory_index] = NULL;

			search_index = 0;
			while (dirs[search_index] != NULL)
			{
				dir_stream = opendir(dirs[search_index]);

				if (dir_stream == NULL)
				{
					search_index++;
					continue;
				}

				found = 0;
				entry = readdir(dir_stream);
				while (entry != NULL)
				{
					if (strcmp(cmd[0], entry->d_name) == 0)
					{
						found = 1;
						break;
					}

					entry = readdir(dir_stream);
				}

				closedir(dir_stream);

				if (found == 1)
					break;

				search_index++;
				if (search_index == directory_index)
				{
					perror(cmd[0]);
				}
			}
		}

		running_pid = fork();
		if (running_pid == -1)
		{
			perror("fork");
		}
		else if (running_pid == 0)
		{
			if (path == NULL)
			{
				size = strlen(dirs[search_index]) + strlen(line) + 2;
				concatenated_path = malloc(size);
				if (concatenated_path == NULL)
				{
					perror("malloc");
				}

				strcpy(concatenated_path, dirs[search_index]);
				strcat(concatenated_path, "/");
				strcat(concatenated_path, cmd[0]);

				cmd[0] = concatenated_path;
				execve(cmd[0], cmd, environ);

				perror("execve");
				free(concatenated_path);
				free(cmd);
			}
			else
			{
				(void)av;
				(void)err_msg;
				execve(cmd[0], cmd, environ);

				perror("execve");
			}

		}

		wait(&status);

		if (path == NULL)
		{
			for (free_dir_index = 0; dirs[free_dir_index] != NULL; free_dir_index++)
			{
				free(dirs[free_dir_index]);
			}
		}

		for (i = 0; cmd[i] != NULL; i++)
		{
			free(cmd[i]);
		}

		free(dirs);
		free(path_environment_variable);
		free(concatenated_path);
		free(cmd);
		free(line);
	}
	free(line);
	if (isatty(STDIN_FILENO) == 1)
		write(1, "\n", 1);
	return (0);
}
