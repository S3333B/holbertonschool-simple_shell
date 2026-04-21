#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;
extern char *shell_name;

char *get_path(char *cmd);
char **tokenize(char *line);
void print_prompt(void);
int handle_eof(ssize_t nread, char *line);
void clean_line(char *line);
void execute_command(char *line);
int handle_builtin(char **argv, char *line);
void execute_external(char **argv);

#endif /* SHELL_H */
