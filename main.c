#include "shell.h"

/**
 * main - simple shell
 *
 * Return: Always 0
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	pid_t pid;
	int status;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			printf("($) ");

		read = getline(&line, &len, stdin);
		if (read == -1)
			break;

		line[strcspn(line, "\n")] = '\0';

		pid = fork();
		if (pid == 0)
		{
			char *argv[] = {line, NULL};

			if (execve(line, argv, NULL) == -1)
				perror("./hsh");
			exit(EXIT_FAILURE);
		}
		else
		{
			wait(&status);
		}
	}

	free(line);
	return (0);
}
