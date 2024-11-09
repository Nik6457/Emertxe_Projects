#include "inverted_search.h"

void save_database( Wlist *head[])
{
    //prompt the user for new file to save the database
    char file_name[FNAME_SIZE];

    printf("Enter the filename to save the database\n");
    scanf("%s" ,file_name);

    //open file in write mode

    FILE *fptr = fopen(file_name , "w");

    int i;
    for(i = 0 ; i<27 ; i++)
    {
	if(head[i] != NULL)
	{
	    write_databasefile(head[i] , fptr);
	}
    }
    printf("Database saved\n");
}

void write_databasefile(Wlist *head, FILE* databasefile)
{
    //...............TODO.........

    //#[0]  word is [are]: file count is 1:file/s: file is file1.txt: word count is 2

    // #[index]      [word]  : [file_count]  :  [file_name]   :  [word_count]

    if (databasefile == NULL)
    {
	printf("Error : File not available\n");
	return;
    }

   // fprintf(databasefile," [index]     [word]    filecount  file/s        file_name          word_count\n");
    //Checking the Wlist empty or not
    while (head != NULL)
    {
	fprintf(databasefile,"#[%d]\n  word is [%s] : file count is %d : file/s : ",tolower(head->word[0]) % 97 ,  head->word, head->file_count);

	Ltable* l_temp = head->Tlink;


	while (l_temp != NULL)
	{
	    fprintf(databasefile,"  file is %s : word count is %d\n",l_temp->file_name, l_temp->word_count);
	    l_temp = l_temp->table_link;
	}
	fprintf(databasefile,"\n");
	//Moving temp to next node
	head = head->link;
    }
  

}


















