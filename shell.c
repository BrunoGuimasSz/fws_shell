#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<sys/wait.h>

#define MAX_TOKENS 10

void clearScreenCommand()
{
    printf("\e[1;1H\e[2J");
}
void echoCommand(char *output)
{
    printf("%s\n", output);
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

    // Strcmp returns 0 if the two strings are equal
    if(!strcmp(tokenList[0], "echoCommand"))
    {
        echoCommand(input);
    }
    if(!strcmp(tokenList[0], "cls"))
    {
        clearScreenCommand();
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
    FILE *historyFile = fopen("history.txt", "w");

    char input[50];

    while(strcmp(input, "exit"))
    {
        printDirectory();
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        checkCommands(input);
    }

    fclose(historyFile);

    return 0;
}
