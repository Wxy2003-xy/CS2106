#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
int main()
{
    //Since largest number is 10 digits, a 12 characters string is more
    //than enough
    int ct, i;
    char cStringExample[12];
    scanf("%d", &ct);
    int* userInput = (int*) malloc (ct * sizeof(int));
    for (i = 0; i < ct; i++) {
        scanf("%d", &userInput[i]);
    }
   
    for (i = 0; i < ct; i++) {
        pid_t childPid = fork();
        if (childPid == 0) {
            char cStringExample[12];
            sprintf(cStringExample, "%d", userInput[i]);
            execl("./PF", "PF", cStringExample, NULL);
            exit(1);
        }
    }
    for (i = 0; i < ct; i++) {
        int childResult;
        wait(&childResult);
        printf("%d has %d prime factors\n", userInput[i], WEXITSTATUS(childResult));
    }
    free(userInput);
}
