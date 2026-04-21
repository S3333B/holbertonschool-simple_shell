#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

/**
 * print_prompt - prints the prompt
 */
void print_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "#cisfun$ ", 9);
}

/**
 * handle_eof - handles EOF condition
 * @nread: return value of getline
 * @line: input buffer
 *
 * Return: 1 if EOF, 0 otherwise
 */
int handle_eof(ssize_t nread, char *line)
{
	if (nread == -1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "\n", 1);
		free(line);
		return (1);
	}
	return (0);
}

/**
 * clean_line - removes newline character
 * @line: input line
 */
void clean_line(char *line)
{
	line[strcspn(line, "\n")] = '\0';
}


/**
 * main - simple shell
 *
 * @argc: number of arguments
 * @argv: vector of arguments
 *
 * Return: Always 0
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	(void)argc;
	shell_name = argv[0];

	while (1)
	{
		print_prompt();
		nread = getline(&line, &len, stdin);

		if (handle_eof(nread, line))
			return (0);

		clean_line(line);

		if (line[0] == '\0')
			continue;

		execute_command(line);
	}
}
