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

void writeCommand(char *tokenList[MAX_TOKENS], int tokenCount)
{
    if(!tokenList[1])
        return;

    FILE *fileToWrite = NULL;
    fileToWrite = fopen(tokenList[1], "w");

    char text[500];

    for(int i = 2; i < tokenCount; i++)
    {
        strcat(text, tokenList[i]);
        strcat(text, " ");
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

    // Strcmp returns 0 if the two strings are equal
    if(!strcmp(tokenList[0], "say"))
        sayCommand(tokenList, i);
    else if(!strcmp(tokenList[0], "clc"))
        clearConsoleCommand();
    else if(!strcmp(tokenList[0], "dog"))
        dogCommand(tokenList);
    else if(!strcmp(tokenList[0], "write"))
        writeCommand(tokenList, i);
    else
        printf("Error: %s: command not found\n", tokenList[0]);
}

void printUserAndDirectory()
{
    char *userName = getlogin();

    char cwd[PATH_MAX];
    const int cwdSize = sizeof(cwd);

    char *currentPath= getcwd(cwd, cwdSize);

    printf("[%s@%s]$ ", userName, currentPath);
}

int main()
{
    FILE *historyFile = NULL;
    historyFile = fopen("history.txt", "a");

    char input[500];
    int inputSize = sizeof(input);

    while(strcmp(input, "exit"))
    {
        printUserAndDirectory();

        fgets(input, inputSize, stdin);
        input[strcspn(input, "\n")] = '\0';

        fprintf(historyFile, "%s\n", input);
        fflush(historyFile);

        checkCommands(input);
    }

    fclose(historyFile);

    return 0;
}
