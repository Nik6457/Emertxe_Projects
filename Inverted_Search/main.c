/*
Name          : Nikhil Paikrao
Date          : 24/03/2024
Project title : Inverted Search
Sample input  : ./calc.exe file1.txt file2.txt
Sample output : Successful: Inserting the file name : file1.txt into file linked list
                Successful: Inserting the file name : file2.txt into file linked list
                Select your choice among following options:
                 1. Create DATABASE
                 2. Display Database
                 3. Update DATABASE
                 4. Search
                 5. Save Database
                Enter your choice: 1
                Do you want to continue (y/n): y

                Select your choice among following options:
                 1. Create DATABASE
                 2. Display Database
                 3. Update DATABASE
                 4. Search
                 5. Save Database
                Enter your choice: 2
                [index]     [word]    file_count  file/s       File: File_name   word_count
                [0]          are           1    file/s:        File : file1.txt   1
                [7]          hi            1    file/s:        File : file1.txt   2
                [7]          hello         1    file/s:        File : file1.txt   1
                [7]          how           1    file/s:        File : file1.txt   1
                [7]          Hello         1    file/s:        File : file1.txt   1
                [8]          is            1    file/s:        File : file2.txt   1
                [8]          inverted      1    file/s:        File : file2.txt   1
                [15]         project.      1    file/s:        File : file2.txt   1
                [18]         search        1    file/s:        File : file2.txt   1
                [19]         this          1    file/s:        File : file2.txt   1
                [24]         you           1    file/s:        File : file1.txt   1
                Do you want to continue (y/n): y

                Select your choice among following options:
                 1. Create DATABASE
                 2. Display Database
                 3. Update DATABASE
                 4. Search
                 5. Save Database
                Enter your choice: 3
                Enter the file for updating the database
                file2.txt
                This file name : file2.txt is repeated. Do not add that file into file linked list
                Do you want to continue (y/n): y

                Select your choice among following options:
                 1. Create DATABASE
                 2. Display Database
                 3. Update DATABASE
                 4. Search
                 5. Save Database
                Enter your choice: 5
                Enter the filename to save the database
                save.txt
                Database saved
                Do you want to continue (y/n): y

                Select your choice among following options:
                 1. Create DATABASE
                 2. Display Database
                 3. Update DATABASE
                 4. Search
                 5. Save Database
                Enter your choice: 4
                Enter the word to search in database
                hi
                Word hi is present in 1 file/s
                In file : file1.txt  2  time/s

                Do you want to continue (y/n): n
*/
#include "inverted_search.h"

int main(int argc , char *argv[])
{
    system("clear");
    Wlist *head[27] = {NULL};
    // validation 
    if(argc <= 1)
    {
	printf("Enter the valid no.of arg\n");
	printf("./Slist.exe file1.txt file2.txt.....\n");
	return 0;
    }

    //create list of filenames 
    //declare head pointer

    Flist *f_head = NULL;

    //validate the files 
    file_validation_n_file_list(&f_head , argv);

    if(f_head == NULL)
    {
	printf("No files are available in the file linked list\n");
	printf("Hence the process terminated\n");
	return 1;
    }
    int choice;
    char option;
    char word[WORD_SIZE];
    do
    {

	printf("Select your choice among following options:\n1. Create DATABASE\n2. Display Database\n3. Update DATABASE\n4. Search\n5. Save Database\nEnter your choice: ");

	scanf("%d", &choice);
	switch(choice)
	{
	    //create
	    case 1:

		create_database(f_head ,head);
		break;
		//display database

	    case 2:
		display_database(head);
		break;

		//update database
	    case 3:

		update_database(head , &f_head);
		break;

		//search database
	    case 4:
		printf("Enter the word to search in database\n");
		scanf("%s" , word);

		search(head[tolower(word[0])%97] ,word);
		break;

		//save database
	    case 5:

		save_database(head);
		break;
	    default: 
		printf("Invalid option");
		break;
	}
	printf("Do you want to continue (y/n): ");
	getchar();
	scanf("%c",&option);
    } while (option == 'Y' || option == 'y');
}













































