#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "limit_seat_shared.h"

void routine(LMTSharedMem* shm, int i) {
    printf("Philosopher %d is hungry.\n");
    sem_wait(&shm->seat);
    sem_wait(&shm->sem[LEFT]);
    sem_wait(&shm->sem[RIGHT]);
    printf("Philosopher %d is eating... \n");
    sleep(1);
    printf("Philosopher %d finishes eating. \n");
    sem_post(&shm->sem[LEFT]);
    sem_post(&shm->sem[RIGHT]);
    sem_post(&shm->seat);
}


int main() {
    int i; 
    int shmid = shmget(IPC_PRIVATE, sizeof(LMTSharedMem), IPC_CREAT | 0600);
    LMTSharedMem* shm = (LMTSharedMem*) shmat(shmid, NULL, 0);
    sem_init(&shm->seat, 1, N-1);
    for (i = 0; i < N; i++) {
        sem_init(&shm->sem[i], 1, 1);
    }
    for (i = 0; i < N; i++) {
        if (fork() == 0) {
            routine(shm, i);
            exit(0);
        }
    }
    for (i = 0; i < N; i++) {
        wait(NULL);
    }
    for (i = 0; i < N; i++) {
        sem_destroy(&shm->sem[i]);
    }
    sem_destroy(&shm->seat);
    shmdt(shm);
    shmctl(shmid, IPC_RMID, 0);
}