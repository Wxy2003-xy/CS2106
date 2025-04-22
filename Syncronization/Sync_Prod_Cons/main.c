#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "share.h"


int main() {
    int i;
    int shmid = shmget(IPC_PRIVATE, sizeof(SharedMem), IPC_CREAT | 0600);
    SharedMem* shm = (SharedMem*) shmat(shmid, NULL, 0);
    sem_init(&shm->notEmpty, 1, 0);
    sem_init(&shm->notFull, 1, BUFFER_SIZE);
    sem_init(&shm->mutex, 1, 1);
    shm->in_idx = 0;
    shm->out_idx = 0;
    char shmid_str[20];
    sprintf(shmid_str, "%d", shmid);    // write shmid to str

    for (i = 0; i < 9; i++) {
        char iter[8];
        sprintf(iter, "%d", i);
        char* prodArg[] = {"./Producer", shmid_str, iter, NULL};
        char* consArg[] = {"./Consumer", shmid_str, iter, NULL};
        if (fork() == 0) {
            execvp(prodArg[0], prodArg);
            exit(1);
        }
        if (fork() == 0) {
            execvp(consArg[0], consArg);
            exit(1);
        }
    }
    for (i = 0; i < 18; i++) wait(NULL);

    shmdt(shm);
    shmctl(shmid, IPC_RMID, 0);
    return 0;
}