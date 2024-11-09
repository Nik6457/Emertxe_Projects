#include "inverted_search.h"

void file_validation_n_file_list(Flist** f_head, char* argv[])
{
    //Read file one by one empty or not
    int i = 1, ret_val, empty;
    while (argv[i] != NULL)
    {
        empty = isFileEmpty(argv[i]);
        if (empty == FILE_NOTAVAILABLE)
        {
            printf("File: %s is not available\n", argv[i]);
	    printf("Hence we are not adding the file into list\n");
            i++;
	        continue;
        }
        else if (empty == FILE_EMPTY)
        {
            printf("File: %s is not having contents in it\n", argv[i]);
	    printf("Hence we are not adding the file into list\n");
            i++;	    
            continue;
        }
        else
        {
            //File valid then  add it to the linked list
            ret_val = to_create_list_of_files(f_head, argv[i]);
            if (ret_val == SUCCESS)
            {
                printf("Successful: Inserting the file name : %s into file linked list\n",argv[i]);
            }
            else if (ret_val == REPEATATION)
            {             		
                printf("This file name : %s is repeated . Do not add that file into file linked list\n", argv[i]);           
            }
            else
            {
                printf("Failure\n");
            }
            i++;
        }
    }
}
//fun() is used to check file availability and file contents
int isFileEmpty(char *filename)
{
    //Opening the file read only mode
    FILE* fptr = fopen(filename, "r");

    //Validating fptr. If fprt NULL then file not available
    if (fptr == NULL)
    {
	if(errno == ENOENT)
	{
        return FILE_NOTAVAILABLE;
	}
    }

    //Check if file empty or not
    fseek(fptr, 0, SEEK_END);
    if (ftell(fptr) == 0)
    {
        return FILE_EMPTY;
    }
    //If ftell(fptr) not 0 that means file has some data so return SUCCESS
    return SUCCESS;
}

/* Inserting the file in Flist */
int to_create_list_of_files (Flist **f_head, char *name)
{
    Flist* temp = *f_head;
    while (temp != NULL)
    {
        if (strcmp(temp->file_name, name) == 0)
        {
            return REPEATATION;
        }
        temp = temp->link;
    }

    //Creating new node
    Flist* new = malloc(sizeof(Flist));
    
    //Validating new
    if (new == NULL)
    {
        return FAILURE;
    }

    //Updating filename and link part
    strcpy(new->file_name, name);
    new->link = NULL;
    if (*f_head == NULL)
    {
        *f_head = new;
         return SUCCESS;
    }

    temp = *f_head;
    while (temp->link != NULL)
    {
        temp = temp->link;
    }

    temp->link = new;
    return  SUCCESS;
}























