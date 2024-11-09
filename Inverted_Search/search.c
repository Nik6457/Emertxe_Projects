#include "inverted_search.h"

 void search( Wlist *head, char *word)
{
    //check empty or not

    if(head == NULL)
    {
	printf("Search is not possible , list is empty \n");
	return;
    }

    //traverse the list

    while(head)
    {
	//compare the search word with each node words available 
	if(!strcmp(head->word , word))
	{
	    printf("Word %s is present in %d file/s\n" , word , head->file_count);
	    Ltable *Thead = head->Tlink;
	    while(Thead)
	    {
		printf("In file : %s  %d  time/s \n" , Thead->file_name , Thead->word_count );
		Thead = Thead->table_link;
	    }
	    printf("\n");
	    return;
	}
	head = head->link;
    }
    printf("Search word not found in list\n");
}







