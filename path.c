#include "shell.h"
#include <errno.h>

/**
 * build_path - builds a full path from a directory and a command
 * @dir: directory path
 * @cmd: command name
 *
 * Return: allocated full path, or NULL on failure
 */
static char *build_path(char *dir, char *cmd)
{
	char *full_path;
	size_t len;

	len = strlen(dir) + strlen(cmd) + 2;
	full_path = malloc(sizeof(char) * len);
	if (full_path == NULL)
		return (NULL);

	snprintf(full_path, len, "%s/%s", dir, cmd);
	return (full_path);
}

/**
 * get_path - finds full path of a command
 * @cmd: command
 *
 * Return: full path or NULL
 */
char *get_path(char *cmd)
{
	char *path, *path_copy, *dir;
	char *full_path;

	if (cmd == NULL || cmd[0] == '\0')
		return (NULL);

	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}

	path = getenv("PATH");
	if (path == NULL)
	{
		errno = ENOENT;
		return (NULL);
	}

	path_copy = strdup(path);
	if (path_copy == NULL)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		full_path = build_path(dir, cmd);
		if (full_path == NULL)
		{
			free(path_copy);
			return (NULL);
		}

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	errno = ENOENT;
	return (NULL);
}
