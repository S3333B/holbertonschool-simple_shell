#include "shell.h"
#include <errno.h>

/**
 * print_command_error - prints an error matching sh format
 * @command: command that failed
 * @shell_name: name used to run the shell
 * @line_number: current input line number
 */
static void print_command_error(char *command, char *shell_name,
	int line_number)
{
	char *message;

	if (errno == EACCES)
		message = "Permission denied";
	else
		message = "not found";

	fprintf(stderr, "%s: %d: %s: %s\n", shell_name, line_number, command,
		message);
}

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
 * wait_for_child - waits for a child process and returns its status
 * @pid: child process id
 * @shell_name: name used to run the shell
 *
 * Return: child exit status
 */
static int wait_for_child(pid_t pid, char *shell_name)
{
	int status;

	if (waitpid(pid, &status, 0) == -1)
	{
		perror(shell_name);
		return (1);
	}

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

/**
 * execute_external - executes external command
 * @argv: arguments
 * @shell_name: name used to run the shell
 * @line_number: current input line number
 *
 * Return: command exit status
 */
int execute_external(char **argv, char *shell_name, int line_number)
{
	pid_t pid;
	int saved_errno, status;
	char *cmd_path;

	cmd_path = get_path(argv[0]);
	if (cmd_path == NULL)
	{
		saved_errno = errno;
		print_command_error(argv[0], shell_name, line_number);
		if (saved_errno == EACCES)
			return (126);
		return (127);
	}

	pid = fork();
	if (pid == -1)
	{
		perror(shell_name);
		free(cmd_path);
		return (1);
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

	status = wait_for_child(pid, shell_name);
	free(cmd_path);
	return (status);
}

/**
 * execute_command - main dispatcher
 * @line: input line
 * @shell_name: name used to run the shell
 * @line_number: current input line number
 *
 * Return: command exit status
 */
int execute_command(char *line, char *shell_name, int line_number)
{
	char **argv;
	int status;

	argv = tokenize(line);
	if (argv == NULL)
		return (1);

	if (argv[0] == NULL)
	{
		free(argv);
		return (0);
	}

	if (handle_builtin(argv, line))
	{
		free(argv);
		return (0);
	}

	status = execute_external(argv, shell_name, line_number);

	free(argv);
	return (status);
}
