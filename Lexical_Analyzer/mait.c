#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_SIZE 100

// Arrays of operators, special characters, and keywords
static char operator[] = {'+', '-', '*', '/', '=', '<', '>', '~', '&', ',', '!', '^', '|'};
static char special_charcter[] = {'(', ')', '{', '}', ';', ':', '[', ']'};
static char *keyword[] = {
    "for", "goto", "return", "if", "else", "do", "while", "continue", "break", "switch",
    "case", "default", "sizeof", "const", "volatile", "extern", "auto", "register",
    "static", "signed", "unsigned", "short", "long", "double", "char", "int", "float",
    "struct", "union", "enum", "void", "typedef", ""};

// Function declarations
void token(char *input);
int is_operator(char opr);
int is_special_character(char special_ch);
int is_keyword(char *token);

// Check if a character is an operator
int is_operator(char opr)
{
    for (int i = 0; i < 13; i++)
    {
        if (operator[i] == opr)
            return 1;
    }
    return 0;
}

// Check if a character is a special character
int is_special_character(char special_ch)
{
    for (int i = 0; i < 8; i++)
    {
        if (special_charcter[i] == special_ch)
            return 1;
    }
    return 0;
}

// Check if a token is a keyword
int is_keyword(char *token)
{
    for (int i = 0; i < 33; i++)
    {
        if (strcmp(token, keyword[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// Main function
int main(int argc, char *argv[])
{
    FILE *fptr;
    char str[100];

    if (argc > 1)
    {
        // Open the file passed via the command line
        fptr = fopen(argv[1], "r");
    }
    else
    {
        printf("INFO: Pass a .c or .txt file\n");
        return 1;
    }

    if (fptr == NULL)
    {
        printf("Failure to open the file %s\n", argv[1]);
        return 1;
    }
    else
    {
        // Process each line of the file
        while (fgets(str, 100, fptr) != NULL)
        {
            token(str);
        }
        fclose(fptr);
    }

    return 0;
}

// Token processing function
void token(char *input)
{
    char token[MAX_SIZE];
    int i = 0, j = 0, k = 0;

    while (input[i] != '\0')
    {
        // Remove spaces
        if (isspace(input[i]))
        {
            i++;
            continue;
        }

        // Handle operators
        if (is_operator(input[i]))
        {
            printf("Operator : %c\n", input[i]);
            i++;
            continue;
        }

        // Handle special characters
        if (is_special_character(input[i]))
        {
            printf("Special Character: %c\n", input[i]);
            i++;
            continue;
        }

        // Handle identifiers and keywords
        if (isalpha(input[i]))
        {
            j = 0;
            while (isalnum(input[i]))
            {
                token[j++] = input[i++];
            }
            token[j] = '\0';

            if (is_keyword(token))
            {
                printf("Keyword : %s\n", token);
            }
            else
            {
                printf("Identifier: %s\n", token);
            }
            continue;
        }

        // Handle constants
        if (isdigit(input[i]))
        {
            k = 0;
            while (isdigit(input[i]))
            {
                token[k++] = input[i++];
            }
            token[k] = '\0';
            printf("Constant : %s\n", token);
            continue;
        }

        // Handle preprocessor directives
        if (input[i] == '#')
        {
            j = 0;
            while (input[i] != '\0' && input[i] != '\n')
            {
                token[j++] = input[i++];
            }
            token[j] = '\0';
            printf("Preprocessor directive: %s\n", token);
            continue;
        }

        // Handle string literals
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
            printf("String literal: %s\n", token);
            continue;
        }

        i++;
    }
}
