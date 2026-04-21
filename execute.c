#include "shell.h"

char *shell_name = "./hsh";

/**
 * handle_builtin - handles built-in commands
 * @argv: arguments
 * @line: input line
 *
 * Return: 1 if handled, 0 otherwise
 */
int handle_builtin(char **argv, char *line)
{
	int i;

	if (strcmp(argv[0], "exit") == 0)
	{
		free(argv);
		free(line);
		exit(0);
	}

	if (strcmp(argv[0], "env") == 0)
	{
		for (i = 0; environ[i] != NULL; i++)
		{
			write(STDOUT_FILENO, environ[i], strlen(environ[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		return (1);
	}

	return (0);
}
/**
 * execute_external - executes external command
 * @argv: arguments
 */
void execute_external(char **argv)
{
	pid_t pid;
	int status;
	char *cmd_path;

	cmd_path = get_path(argv[0]);
	if (cmd_path == NULL)
	{
		perror(shell_name);
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		perror(shell_name);
		free(cmd_path);
		return;
	}

	if (pid == 0)
	{
		if (execve(cmd_path, argv, environ) == -1)
		{
			perror(shell_name);
			free(cmd_path);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			perror(shell_name);
	}

	free(cmd_path);
}

/**
 * execute_command - main dispatcher
 * @line: input line
 */
void execute_command(char *line)
{
	char **argv;

	argv = tokenize(line);
	if (argv == NULL)
		return;

	if (argv[0] == NULL)
	{
		free(argv);
		return;
	}

	if (handle_builtin(argv, line))
	{
		free(argv);
		return;
	}

	execute_external(argv);

	free(argv);
}
