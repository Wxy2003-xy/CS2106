#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    fork();
    fork();
    if (fork() == 0) {
        exit(0);
    }
    fork();
    printf("a\n");
}