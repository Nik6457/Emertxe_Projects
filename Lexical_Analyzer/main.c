#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_SIZE 300


static char operator[] = {'+', '-', '*', '/', '=', '<', '>','~', '&', ',', '!', '^', '|'};
static char special_charcter[] = {'(' ,')', '{', '}', ';', ':', '[', ']'};
static char *keyword[] = {"for", "goto", "return", "if", "else", "do", "while", "continue", "break", "switch", "case", "default", "sizeof", "const", "volatile", "extern", "auto", "register", "static", "signed", "unsigned", "short", "long", "double", "char", "int", "float", "struct", "union", "enum", "void", "typedef", ""};

void token(char *input);

// check if charcter is an operator
int is_operator(char opr)
{
    int i;
    for(i = 0; i <= 13; i++)
    {
	if(operator[i] == opr)
	    return 1;
    }
    return 0;
}

// check if charcter is a special charcter
int is_special_character(char special_ch)
{
    int i;
    for(i = 0; i < 8; i++)
    {
	if(special_charcter[i] == special_ch)
	    return 1;
    }
    return 0;
}

// check if charcter is a keyword 
int is_keyword(char *token)
{
    int i;
    for(i = 0; i < 33; i++)
    {
	if(strcmp(token, keyword[i]) == 0)
	{
	    return 1;
	}
    }
    return 0;

}


int main(int argc, char *argv[])
{
    FILE *fptr;
    char str[100];

    int i = 1;

    // argument validation
    if(argv[i] != NULL)
    {

	fptr = fopen("file.c", "r");
    }
    else
    {
	printf("INFO: Pass .c or .txt file\n");
	return 1;
    }

    if(fptr == NULL)
    {
	printf("Failure to open the file.txt\n");
	return 1;
    }
    else
    {
	while(fgets(str, 100, fptr) > 0)
	{
	    token(str);
	}

    }


    return 0;
}

void token(char *input)
{
    char token[MAX_SIZE];

    int i = 0, j = 0, k = 0;

    while(input[i] != '\0')
    {
	// remove spaces
	if(isspace(input[i]))
	{
	    i++;
	    continue;
	}

	// Handle the operators
	if(is_operator(input[i]))
	{ 
	    printf("Operator               : %c\n", input[i]);
	    i++;
	    continue;   
	}

	// Handle the special charcter
	if(is_special_character(input[i]))
	{
	    printf("Special Character      : %c\n", input[i]);
	    i++;
	    continue;
	}



	// Handle identifier and keywords
	if(isalpha(input[i]))
	{
	    j = 0;

	    while(isalnum(input[i]))
	    {
		token[j++] = input[i++];
	    }
	    token[j] = '\0';

	    if(is_keyword(token))
	    {
		printf("Keyword                : %s\n", token);
	    }
	    else
	    {
		printf("Identifier             : %s\n", token);
	    }
	    continue;

	}


	// Handle numeric constant
	if(isdigit(input[i]))
	{
	    k = 0;
	    while(isdigit(input[i]))
	    {
		token[k++] = input[i++];
	    }
	    token[k] = '\0';

	    printf("Constant               : %s\n", token);
	    continue;
	}


	// Handle pre-proccessor directive
	if (input[i] == '#')
	{
	    j = 0;
	    while (input[i] != '\0' && input[i] != '\n')
	    {
		token[j++] = input[i++];
	    }
	    token[j] = '\0';
	    printf("Preprocessor directive : %s\n", token);
	    continue;
	}


	// Handle string literal
	if (input[i] == '"')
	{
	    j = 0;
	    token[j++] = input[i++];  // Capture the opening double quote
	    while (input[i] != '\0' && input[i] != '"')
	    {
		token[j++] = input[i++];
	    }
	    if (input[i] == '"')  // Capture the closing double quote
	    {
		token[j++] = input[i++];
	    }
	    token[j] = '\0';
	    printf("String literal         : %s\n", token);
	    continue;
	}

	i++;

    }

}



