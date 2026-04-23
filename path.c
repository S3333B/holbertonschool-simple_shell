#include "shell.h"
#include <errno.h>
#include <sys/stat.h>

/**
 * get_env_value - gets the value for a given environment variable
 * @name: environment variable name
 *
 * Return: pointer to the value, or NULL if missing
 */
static char *get_env_value(char *name)
{
	int i;
	size_t len;

	len = strlen(name);
	for (i = 0; environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return (environ[i] + len + 1);
	}

	return (NULL);
}

/**
 * build_path - builds a full path from a directory slice and a command
 * @dir: directory path
 * @dir_len: directory length
 * @cmd: command name
 *
 * Return: allocated full path, or NULL on failure
 */
static char *build_path(const char *dir, size_t dir_len, char *cmd)
{
	char *full_path;
	size_t cmd_len, len;
	const char *actual_dir;
	size_t i;

	actual_dir = dir;
	if (dir_len == 0)
	{
		actual_dir = ".";
		dir_len = 1;
	}

	cmd_len = strlen(cmd);
	len = dir_len + cmd_len + 2;
	full_path = malloc(sizeof(char) * len);
	if (full_path == NULL)
		return (NULL);

	for (i = 0; i < dir_len; i++)
		full_path[i] = actual_dir[i];
	full_path[dir_len] = '/';
	for (i = 0; i < cmd_len; i++)
		full_path[dir_len + 1 + i] = cmd[i];
	full_path[len - 1] = '\0';

	return (full_path);
}

/**
 * is_executable - checks whether a path can be executed
 * @path: full path to inspect
 *
 * Return: 1 if executable, 0 otherwise
 */
static int is_executable(char *path)
{
	struct stat st;

	if (stat(path, &st) == -1)
		return (0);

	if (S_ISDIR(st.st_mode))
	{
		errno = EACCES;
		return (0);
	}

	if (access(path, X_OK) == 0)
		return (1);

	return (0);
}

/**
 * search_path - searches PATH directories for a command
 * @path: PATH environment value
 * @cmd: command
 *
 * Return: full path or NULL
 */
static char *search_path(char *path, char *cmd)
{
	char *full_path, *start, *end;
	int saved_errno;

	saved_errno = ENOENT;
	start = path;
	while (1)
	{
		end = start;
		while (*end != '\0' && *end != ':')
			end++;

		full_path = build_path(start, end - start, cmd);
		if (full_path == NULL)
			return (NULL);

		if (is_executable(full_path))
			return (full_path);

		if (errno == EACCES)
			saved_errno = EACCES;

		free(full_path);
		if (*end == '\0')
			break;
		start = end + 1;
	}

	errno = saved_errno;
	return (NULL);
}

/**
 * get_path - finds full path of a command
 * @cmd: command
 *
 * Return: full path or NULL
 */
char *get_path(char *cmd)
{
	char *path;

	if (cmd == NULL || cmd[0] == '\0')
		return (NULL);

	if (strchr(cmd, '/'))
	{
		if (is_executable(cmd))
			return (strdup(cmd));
		return (NULL);
	}

	path = get_env_value("PATH");
	if (path == NULL)
	{
		errno = ENOENT;
		return (NULL);
	}

	return (search_path(path, cmd));
}
