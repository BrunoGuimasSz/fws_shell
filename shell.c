#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <dirent.h>

#define MAX_TOKENS 100

const int CHAR_SIZE = sizeof(char);

void writeCommand(char *tokenList[MAX_TOKENS], int tokenCount, int inputSize)
{
    if(!tokenList[1])
        return;
        
    FILE *fileToWrite = NULL;
    fileToWrite = fopen(tokenList[1], "w");
    
    char *text = (char*)malloc(inputSize);
    
    for(int i = 2; i < tokenCount; i++)
    {
        strcat(text, tokenList[i]);
    }
    
    fprintf(fileToWrite, "%s", text);
    
    fclose(fileToWrite);
}
void dogCommand(char *tokenList[MAX_TOKENS])
{
    if(!tokenList[1])
        return;

    FILE *fileToDog = NULL;
    fileToDog = fopen(tokenList[1], "r");

    if(!fileToDog)
    {
        printf("File do not exist\n");
        return;
    }

    char fileToBeRead[500];
    
    while(fgets(fileToBeRead, 500, fileToDog))
    {
        printf("%s", fileToBeRead);
    }
    
    printf("\n");

    fclose(fileToDog);
}
void clearConsoleCommand()
{
    printf("\e[1;1H\e[2J");
}

void sayCommand(char *tokenList[MAX_TOKENS], int tokenCount)
{
    if(!tokenList[1])
        return;
    for(int i = 1; i < tokenCount; i++)
    {
        if(tokenList[i])
            printf("%s ", tokenList[i]);
    }

    printf("\n");
}

void checkCommands(char *input)
{
    char *token = strtok(input, " ");
    char *tokenList[MAX_TOKENS];
    int i = 0;

    while(token && i < MAX_TOKENS)
    {
        tokenList[i] = token;
        token = strtok(NULL, " ");
        i++;
    }

    tokenList[i] = NULL;
    int inputSize = sizeof(input);
    
    // Strcmp returns 0 if the two strings are equal
    if(!strcmp(tokenList[0], "say"))
    {
        sayCommand(tokenList, i);
    }
    else if(!strcmp(tokenList[0], "clc"))
    {
        clearConsoleCommand();
    }
    else if(!strcmp(tokenList[0], "dog"))
    {
        dogCommand(tokenList);
    }
    else if(!strcmp(tokenList[0], "write"))
    {
        writeCommand(tokenList, i, inputSize);
    }
}

void printDirectory()
{
    char *userName = getlogin();
    char cwd[PATH_MAX];
    char *currentPath= getcwd(cwd, sizeof(cwd));

    printf("[%s@%s]$ ", userName, currentPath);
}

int main()
{
    FILE *historyFile = NULL;
    historyFile = fopen("history.txt", "a");

    char input[500];

    while(strcmp(input, "exit"))
    {
        printDirectory();
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        fprintf(historyFile, "%s\n", input);

        checkCommands(input);
    }

    fclose(historyFile);

    return 0;
}