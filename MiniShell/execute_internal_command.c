#include "main.h"

int exit_status;
int job_count;
Job jobs[MAX_JOBS];
int shell_pgid;
int shell_terminal;

void add_job(pid_t pid, char *cmd) 
{
    if (job_count < MAX_JOBS) 
    {
        jobs[job_count].pid = pid;

        strcpy(jobs[job_count].cmd, cmd);

        jobs[job_count].running = 1;

        job_count++;
    }
}


void remove_job(pid_t pid) 
{
    for (int i = 0; i < job_count; i++) 
    {
        if (jobs[i].pid == pid) 
	{
            for (int j = i; j < job_count - 1; j++) 
	    {
                jobs[j] = jobs[j + 1];
            }
            job_count--;
            break;
        }
    }
}



//status of running and stoped process 
void list_jobs() 
{
    for (int i = 0; i < job_count; i++) 
    {
        printf("[%d] %s %s\n", i + 1, jobs[i].running ? "Running" : "Stopped", jobs[i].cmd);
    }
}


//execute internal commands
void execute_internal_command(char *input)
{
    char *cmd = strtok(input, " ");

    if(strcmp(cmd, "cd") == 0) 
    {
	char *dir = strtok(NULL, " ");
	if(dir == NULL) 
	{
	    //if no argument given afer cd cmd
	    fprintf(stderr, "cd: missing argument\n");
	    exit_status = FAILURE;
	}
	else
	{
	    if(chdir(dir) != 0)
	    {
		//if dir not found print --> cd failed: No such file or directory
		perror("cd failed");
		exit_status = FAILURE;
	    }
	    else
	    {
		//change directory
		exit_status = SUCCESS;
	    }
	}
    }

    else if(strcmp(cmd, "pwd") == 0)
    {
	char cwd[1024];
	//getcwd for getting current working directory, if it is not NULL print current working directory
	if(getcwd(cwd, sizeof(cwd)) != NULL)
	{
	    printf("%s\n", cwd);
	    exit_status = SUCCESS;
	}
	else
	{
	    //failed to get current working directory
	    perror("pwd failed");
	    exit_status = FAILURE;
	}
    }

    else if(strcmp(cmd, "mkdir") == 0)
    {
	char *dir = strtok(NULL, " ");
	if(dir == NULL)
	{
	    fprintf(stderr, "mkdir: missing arguments\n");
	    exit_status = FAILURE;
	}
	else
	{
	    if(mkdir(dir, 0777) != 0)
	    {
		perror("mkdir failed");
		exit_status = FAILURE;
	    }
	    else
	    {
		exit_status = SUCCESS;
	    }
	}
    }

    else if(strcmp(cmd, "rmdir") == 0)
    {
	char *dir = strtok(NULL, " ");
	if(dir == NULL)
	{
	    //if no agrgument given after rmdir 
	    fprintf(stderr, "rmdir: missing arguments\n");
	    exit_status = FAILURE;
	}
	else
	{
	    if(rmdir(dir) != 0)
	    {
		//No such file or directory found then print error
		perror("rmdir failed");
		exit_status = FAILURE;
	    }
	    else
	    {
		//remove directory
		exit_status = SUCCESS;
	    }
	}
    }

    //print the status of running and stopped process
    else if(strcmp(cmd, "jobs") == 0)
    {
	list_jobs();
	exit_status = SUCCESS;
    }

    else if(strcmp(cmd, "fg") == 0)
    {
	char *job_id_str = strtok(NULL, " ");

	if(job_id_str == NULL)
	{
	    fprintf(stderr, "fg: missing argument\n");
	    exit_status = FAILURE;
	}
	else
	{
	    int job_id = atoi(job_id_str) - 1;

	    if(job_id >= 0 && job_id < job_count)
	    {
		//get and set terminal foreground process group
		tcsetpgrp(shell_terminal, jobs[job_id].pid); 

		//send a sigal to continue if stopped
		kill(jobs[job_id].pid, SIGCONT);

                //wait for changing the state and return status of stopped and running state
		waitpid(jobs[job_id].pid, &exit_status, WUNTRACED);

		tcsetpgrp(shell_terminal, shell_pgid);

		if(WIFEXITED(exit_status) || WIFSIGNALED(exit_status))
		{
		    remove_job(jobs[job_id].pid);
		}
		exit_status = SUCCESS;
	    }
	    else
	    {
		//print failure message
		fprintf(stderr, "fg: no such job\n");
		exit_status = FAILURE;
	    }
	}
    }

    //run background process
    else if(strcmp(cmd, "bg") == 0)
    {
	char *job_id_str = strtok(NULL, " ");
	if(job_id_str == NULL)
	{
	    fprintf(stderr, "bg: missing argument\n");
	    exit_status = FAILURE;
	}
	else
	{
	    int job_id = atoi(job_id_str) - 1;

	    if (job_id >= 0 && job_id < job_count) 
	    {
                kill(jobs[job_id].pid, SIGCONT);

                jobs[job_id].running = 1;

                exit_status = SUCCESS;
            } 
	    else 
	    {
                fprintf(stderr, "bg: no such job\n");
                exit_status = FAILURE;
            }
        }
    }

    else
    {
        fprintf(stderr, "%s: command not found\n", cmd);
        exit_status = FAILURE;
    }


}

