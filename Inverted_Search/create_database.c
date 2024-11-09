#include "inverted_search.h"

char *fname;
void create_database(Flist *f_head, Wlist *head[])
{
    //traverse through the file linked list

    while(f_head)
    {
        read_datafile(f_head,head,f_head->file_name);
        f_head = f_head->link;
    }
}

//read the contents of the file
Wlist *read_datafile(Flist *file, Wlist *head[], char *filename)
{
    fname = filename;

    int flag = 1;
    //open file in read mode
    FILE *fptr = fopen(filename , "r");

    //declare an array to store words
    char word[WORD_SIZE];

    while(fscanf(fptr , "%s" ,word) != EOF)
    {
        //index

        int index = tolower(word[0]) % 97;

        //other than alphabets
        if(!(index >= 0 && index <= 25))
            index = 26;

        //check words are repeated or not
        if(head[index] != NULL)
        {
            Wlist *temp = head[index];

            //compare the words at each node with new word
            while(temp)
            {
                if(!strcmp(temp->word , word))
                {
                    update_word_count(&temp , filename);
                    flag=0;
                    break;
                }
                temp = temp->link;
            }
        }
        if(flag == 1)
        {
            //if wrods are not repeated
            insert_at_last(&head[index] , word);
        }
    }

}
int update_word_count(Wlist **head, char *filename)
{
    Ltable *temp = (*head)->Tlink;
    Ltable *prev = NULL;

    //check whether filenames same or not
    while (temp != NULL)
    {
        if(!strcmp(temp->file_name, filename))
        {
            temp->word_count = temp->word_count + 1;
            return SUCCESS;
        }
        prev = temp;
        temp = temp->table_link;
    }

    //Increament and upadte the file count
    (*head)->file_count = (*head)->file_count + 1;

    //adding new node in link table
    Ltable *new = malloc(sizeof(Ltable));

    if(new == NULL)
    {
        return FAILURE;
    }

    //update link table value
    strcpy(new->file_name, filename);
    new->word_count = 1;
    new->table_link = NULL;

    //establish link between prev and new
    prev->table_link = new;
    return SUCCESS;

}

















