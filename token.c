#include "shell.h"

/**
 * count_tokens - counts the number of tokens in a line
 * @line: input line copy
 *
 * Return: number of tokens
 */
static size_t count_tokens(char *line)
{
	char *token;
	size_t count;

	count = 0;
	token = strtok(line, " \t\n");
	while (token != NULL)
	{
		count++;
		token = strtok(NULL, " \t\n");
	}

	return (count);
}

/**
 * tokenize - splits a line into arguments
 * @line: input line
 *
 * Return: array of arguments
 */
char **tokenize(char *line)
{
	char **args;
	char *line_copy;
	char *token;
	size_t count, i;

	line_copy = strdup(line);
	if (line_copy == NULL)
		return (NULL);

	count = count_tokens(line_copy);
	free(line_copy);

	args = malloc(sizeof(char *) * (count + 1));
	if (args == NULL)
		return (NULL);

	i = 0;
	token = strtok(line, " \t\n");
	while (token != NULL)
	{
		args[i++] = token;
		token = strtok(NULL, " \t\n");
	}
	args[i] = NULL;

	return (args);
}
