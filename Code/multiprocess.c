#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int x = 10, status;
    if (fork() == 0) {
        x += 5;
    }
    else {
        wait(&status);
        x -= 5;
        if (fork() != 0) {
            x += 2;
        }
        wait(&status);
    }
    printf("%d, %d\n", x, status);
}