#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

char *get_path(char *cmd);
char **tokenize(char *line);
void print_prompt(void);
int handle_eof(ssize_t nread, char *line);
void clean_line(char *line);
int execute_command(char *line, char *shell_name, int line_number);
int handle_builtin(char **argv, char *line);
int execute_external(char **argv, char *shell_name, int line_number);

#endif /* SHELL_H */
