#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main( ) {
    //This is process P
    printf("P: PID[%d]\n", getpid());
    if ( fork() == 0 ){
        //This is process Q
        printf("Q: PID[%d]\n", getpid());
        if ( fork() == 0 ) {
            //This is process R
            printf("R: PID[%d]\n", getpid());
            return 0;
        }
        // <Point α>
        // wait(NULL); //* if wait here, Q waits for R to terminate
        // execl(vaild_exe); (no wait here) //^ Q does not wait for R; R can terminate whenever, P waits for Q if the beta has wait(NULL)
    }
    // <Point β>
    wait(NULL); //* if wait here, P waits for Q to terminate
                //& if both wait, P waits for Q waits for R

    return 0;
}