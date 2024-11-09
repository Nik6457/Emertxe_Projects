#include "main.h"

char *internal_cmd[] = {"cd", "pwd", "fg", "bg", "jobs", NULL};
int check_command_type(char *input)
{
    //input : cd dir

    char cmd[strlen(input) + 1];
    int i;
   for(i = 0; input[i] != '\0' && input[i] != ' '; i++)
   {
       cmd[i] = input[i];
   }
   cmd[i] = '\0';

    for(i = 0;internal_cmd[i] != NULL; i++)
    {
	if(strcmp(cmd, internal_cmd[i]) == 0)
	{
	    return INTERNAL;
	}
    }
    return EXTERNAL;

}

