#include "main.h"

/*
 * 1. Prompt (customizable)
 * 2. Special variable ($?, $$, $SHELL)
 */

int exit_status;
int special_variable(char *input)
{
    // input : echo $$

    if(!strncmp(input, "echo ", 5))
    {
	if(!strcmp(input + 5, "$?")) //Display exit status
	{
	    printf("%d\n", exit_status);
	}
	else if(!strcmp(input + 5, "$$"))
	{
	    printf("%d\n", getpid()); //Display pid
	}
	else if(strcmp(input + 5, "$SHELL") == 0)
	{
	    printf("%s\n", get_current_dir_name());
	}
	else
	{
	    printf("%s\n", input + 5);
	}
	return SUCCESS;
    }
    return FAILURE;
}
int main()
{
    char prompt[50] = "minishell$", input_string[100];
    int ret;

    while(1)
    {
    printf("%s", prompt); // Display the prompt
    fgets(input_string, 100, stdin);

    input_string[strlen(input_string) - 1] = '\0'; //Deleting '\n' from the input

     if(!strcmp(input_string, "exit"))
     {
	 exit(0);
     }
     else if(!strncmp(input_string,"PS1=", 4)) //input : PS1=NEW_PROMPT$
     {
	 strcpy(prompt, input_string + 4);
     }
     else if(special_variable(input_string) == SUCCESS)
     {
	 continue;
     }
     else if(ret = check_command_type(input_string) == EXTERNAL) //input_string = ls -l
     {    int background = 0;
            if (input_string[strlen(input_string) - 1] == '&')
            {
                background = 1;
                input_string[strlen(input_string) - 1] = '\0'; // Remove '&' character
            }

	 execute_external_command(input_string, background);
     }
     else if(ret = INTERNAL)
     {
	 execute_internal_command(input_string);
	 continue;
     }


    }





    return 0;
}

