#include "shell.h"

/**
 * tokenize - splits a line into arguments
 * @line: input line
 *
 * Return: array of arguments
 */
char **tokenize(char *line)
{
	char **args;
	char *token;
	int i = 0;

	args = malloc(sizeof(char *) * 64);
	if (args == NULL)
		return (NULL);

	token = strtok(line, " ");
	while (token != NULL)
	{
		args[i++] = token;
		token = strtok(NULL, " ");
	}
	args[i] = NULL;

	return (args);
}
