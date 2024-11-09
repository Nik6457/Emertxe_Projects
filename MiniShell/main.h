#ifndef MAIN_H
#define MAIN_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>

#define INTERNAL 1
#define EXTERNAL 0
#define SUCCESS 0
#define FAILURE 1
#define MAX_JOBS 100

typedef struct {
    pid_t pid;
    char cmd[100];
    int running;
} Job;

extern int exit_status;
extern Job jobs[MAX_JOBS];
extern int job_count;
extern pid_t shell_pgid;
extern int shell_terminal;

int check_command_type(char *input);
void execute_internal_command(char *input);
void execute_external_command(char *input, int background);


#endif // MAIN_H
