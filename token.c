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
	char **tmp;
	char *token;
	size_t i = 0, capacity = 8;

	args = malloc(sizeof(char *) * capacity);
	if (args == NULL)
		return (NULL);

	token = strtok(line, " \t");
	while (token != NULL)
	{
		if (i + 1 >= capacity)
		{
			capacity *= 2;
			tmp = realloc(args, sizeof(char *) * capacity);
			if (tmp == NULL)
			{
				free(args);
				return (NULL);
			}
			args = tmp;
		}

		args[i++] = token;
		token = strtok(NULL, " \t");
	}
	args[i] = NULL;

	return (args);
}
