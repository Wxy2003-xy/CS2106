#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Pair;

Pair PF(int userInput) {
    int nFactor, factor;
    nFactor = 0;
    factor = 2;

    //quick hack to get the number of prime factors
    // only for positive integer
    while (userInput > 1){
        if (userInput % factor == 0){
            userInput /= factor;
            nFactor++;
        } else {
            factor++;
        }
    }
    Pair res = {userInput, nFactor};
    return res;
}

int main( int argc, char* argv[])
{
    //Since largest number is 10 digits, a 12 characters string is more
    //than enough
    int ct, i;
    char cStringExample[12];                                
    scanf("%d", &ct);
    int* userInput = (int*) malloc (ct * sizeof(int));
    pid_t* childPidSequence = (pid_t*) malloc (ct * sizeof(pid_t)); // to record the order in which child processes were created
    for (i = 0; i < ct; i++) {
        scanf("%d", &userInput[i]);
    }
    int true_parallel = atoi( argv[1] ); 
    for (i = 0; i < ct; i++) {                              // for each input open a new child process
        pid_t childPid = fork();                            // get the pid of child process. pid_t is just int aliased 
        childPidSequence[i] = childPid;
        if (childPid == 0) {                                // if this is the new child process created 
            char cStringExample[12];            
            sprintf(cStringExample, "%d", userInput[i]);    // copy parameters to child process
            execl("./PF", "PF", cStringExample, NULL);      // call executable in childprocess. Child process terminate here
            exit(EXIT_FAILURE);                             // only run in case of execl fails                               
        }
    }
    if (true_parallel == 1) {   
        printf("Order Not enforced: \n");
        for (i = 0; i < ct; i++) {          
            int childResult, childPid;                          // This will not enforce order
            childPid = waitpid(-1, &childResult, 0);            // Wait for any child process to finish
            int idx = -1;
            for (int j = 0; j < ct; j++) {                      // Find corresponding index in userInput. for demo purpose, unnecessary
                if (childPidSequence[j] == childPid) {
                    idx = j;
                    break;
                }
            }
            printf("%d has %d prime factors\n", userInput[idx], WEXITSTATUS(childResult));
        }
    } else if (true_parallel == 0) {
        printf("Order enforced: \n");
        for (i = 0; i < ct; i++) {                            
            int childResult, childPid;         
            childPid = waitpid(childPidSequence[i], &childResult, 0); // for each input wait for its child process to finish and return result in the order which child processes were created
            printf("%d has %d prime factors\n", userInput[i], WEXITSTATUS(childResult));
        }
    } else {
        printf("Invalid mode");
    }
    free(userInput);
    return 0;
}
