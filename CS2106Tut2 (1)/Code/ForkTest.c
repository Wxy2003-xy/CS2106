#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h>
int dataX = 100;              
int main(int argc, char *argv[ ]) {
    pid_t childPID;
    int dataY = 200;              
    int* dataZptr = (int*) malloc(sizeof(int));
    *dataZptr = 300;
    //First Phase
    printf("PID[%d] | X = %d | Y = %d | Z = %d |\n", 
            getpid(),  dataX, dataY, *dataZptr);
    //Second Phase
    childPID = fork();
    printf("*PID[%d] | X = %d | Y = %d | Z = %d |\n", 
            getpid(),  dataX, dataY, *dataZptr);
    dataX += 1;
    dataY += 2;
    if(childPID == 0) {
        (*dataZptr) += 3;
    }
    else {
        (*dataZptr) += 5;
        printf("#PID[%d] | X = %d | Y = %d | Z = %d |\n", 
            getpid(),  dataX, dataY, *dataZptr);
    }
    //Insertion Point
    //Part (f)
    //if (childPID == 0)
    //    sleep(5);
    //Part (g)
    if (childPID != 0)
        wait(NULL);
    //Third Phase
    childPID = fork();
    printf("**PID[%d] | X = %d | Y = %d | Z = %d |\n", 
            getpid(),  dataX, dataY, *dataZptr);
    dataX += 1;
    dataY += 2;
    (*dataZptr) += 3;
    printf("##PID[%d] | X = %d | Y = %d | Z = %d |\n", 
            getpid(),  dataX, dataY, *dataZptr);
    
    return 0;
}



/*
a. What is the difference between the 3 variables: dataX, dataY and the memory
location pointed by dataZptr?
^- dataX is a global variable stored in data memory;
^- dataY is local variable stored in the stack frame of main();
^- dataZptr points to a dynamically allocated mem in heap memory;

b. Focusing on the messages generated by second phase (they are prefixed with
either "*" and "#"), what can you say about the behavior of the fork() system
call?
^- the variables in processes created by fork() are copies of the original variables 
^- in the parent process, thus they are modified indenpendently across different children 
^- except the PID 
c. Using the messages seen on your system, draw a process tree to represent the
processes generated. Use the process tree to explain the values printed by the child
processes.
^        P (X, Y, Z)            → 100, 200, 300
^        ├── Q (X, Y, Z)        → 100, 200, 300
^        │   ├── R (X, Y, Z)    → 101, 202, 303
^        └── S (X, Y, Z)        → 101, 202, 305

d. Do you think it is possible to get different ordering between the output messages,
why?
^ Yes. Sibling processes run independently and which outputs first depends on OS scheduling.

e. Can you point how which pair(s) of messages can never swap places? i.e. their
relative order is always the same?
^ Processes of the same PID (same) process cannot change order. 

f. If we insert the following code at the insertion point:

*/