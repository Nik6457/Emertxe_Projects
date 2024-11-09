#include "inverted_search.h"

void update_database( Wlist *head[], Flist **f_head)
{
    //prompt the user

    char file_name[FNAME_SIZE];
    int empty;
    printf("Enter the file for updating the database\n");
    scanf("%s" , file_name);

    //validate

    empty = isFileEmpty(file_name);
    if(empty == FILE_NOTAVAILABLE)
    {
        printf("File : %s is not available\n", file_name);
        printf("Hence we are not adding the file into file list\n");
    }
    else if(empty == FILE_EMPTY)
    {
        printf("File : %s is not having contents in it\n", file_name);
        printf("Hence we are not adding the file into file list\n");
    }
    else
    {
        int ret_val = to_create_list_of_files(f_head, file_name);
        if(ret_val == SUCCESS)
        {
            printf("Successfull : inserting the file name : %s into filelinked list\n" , file_name);
        }
        else if(ret_val == REPEATATION)
        {
            printf("This file name : %s is repeated. Do not add that file into file linked list\n", file_name);

        }
        else
        {
            printf("Failure\n");

        }
    }

    //create the database

    while(*f_head)
    {
        if(!strcmp((*f_head)->file_name , file_name))
        {
            create_database(*f_head, head);
            return;
        }
        *f_head = (*f_head)->link;
    }
    printf("Database update successfully!!\n");
}


