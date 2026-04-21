#include "shell.h"
#include <sys/stat.h>

/**
 * get_path - finds full path of a command
 * @cmd: command
 *
 * Return: full path or NULL
 */
char *get_path(char *cmd)
{
	char *path, *path_copy, *dir;
	char full_path[1024];
	struct stat st;

	if (cmd == NULL)
		return (NULL);

	/* If command already has '/' */
	if (strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0)
			return (strdup(cmd));
		return (NULL);
	}

	path = getenv("PATH");
	if (path == NULL)
		return (NULL);

	path_copy = strdup(path);
	if (path_copy == NULL)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		sprintf(full_path, "%s/%s", dir, cmd);

		if (stat(full_path, &st) == 0)
		{
			free(path_copy);
			return (strdup(full_path));
		}
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}
