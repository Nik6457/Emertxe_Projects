#include "main.h"

extern int exit_status;
void execute_external_command(char *input, int background)
{
    // 1. create a child process

    int pid = fork();

    if(pid == 0)
    {
	//input : ls -l

	//char *cmd[] = {"ls", "-l", NULL};

	//TODO: Write a logic to convert string into array of null terminated strings

	// int execvp(const char *filename, const char *argv[]);i

	char *args[100];
	int i;
	char *token = strtok(input, " ");

	 i = 0;

	//logic of converting string into array of null terminated string
	while(token != NULL)
	{
	    args[i++] = token;
	    token = strtok(NULL, " ");
	}
	args[i] = NULL;

	if(execvp(args[0], args) == -1)
	{
	    printf("%s : Command not found\n", args[0]);
	}

    }
    else if(pid > 0)
    {
	if (background)
        {
            add_job(pid, input);
            printf("[%d] %d\n", job_count, pid);
        }
	else
	{
	
	int status;
	//Parent should wait for the pid
	waitpid(pid, &status, WUNTRACED);

	//Check whether child terminated normally or abnormally

	if(WIFEXITED(status))
	{
	    printf("The child %d terminated normally with code %d\n", pid,(exit_status = WEXITSTATUS(status)));
	}
	else if(WIFSIGNALED(status))
	{
	    exit_status = WTERMSIG(status);
	    printf("The child %d terminated abnormally by receiving the signal %d\n", pid, exit_status);
	    exit_status += 128;
	}
	else if(WIFSTOPPED(status)) //True if the process stopped
	{
	    exit_status = WSTOPSIG(status);
	    printf("The child %d stopped by receiving the signal %d\n", pid, exit_status);
	    exit_status += 128;
	}
	}
	
    }

}
