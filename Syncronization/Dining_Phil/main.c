#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "shared.h"

void takeChpStk(SharedMem* shm, int i) {
    sem_wait(&shm->mutex);
    shm->_status[i] = HUNGRY;
    printf("Philosopher %d is hungry..\n", i);
    safeToEat(shm, i);
    sem_post(&shm->mutex);
    sem_wait(&shm->sem[i]);
}

void safeToEat(SharedMem* shm, int i) {
    if ((shm->_status[i] == HUNGRY) &&
        (shm->_status[LEFT] != EATING) &&
        (shm->_status[RIGHT] != EATING)) {
            shm->_status[i] = EATING;
            printf("Philosopher %d is eating...\n", i);
            sem_post(&shm->sem[i]);
        }
} 

void putChpStk(SharedMem* shm, int i) {
    sem_wait(&shm->mutex);
    shm->_status[i] = THINKING;
    printf("Philosopher %d finishes eating.\n", i);
    safeToEat(shm, LEFT);
    safeToEat(shm, RIGHT);
    sem_post(&shm->mutex);
}

int main() {
    int i, shmid;
    SharedMem* shm;
    shmid = shmget(IPC_PRIVATE, sizeof(SharedMem), IPC_CREAT | 0600);
    shm = (SharedMem*) shmat(shmid, NULL, 0);
    sem_init(&shm->mutex, 1, 1);
    for (i = 0; i < N; i++) {
        shm->_status[i] = THINKING;
        sem_init(&shm->sem[i], 1, 0);
    }
    for (i = 0; i < N; i++) {
        if (fork() == 0) {
            takeChpStk(shm, i);
            putChpStk(shm, i);
            exit(0);
        }
    }
    for (i = 0; i < N; i++) {
        wait(NULL);
    }
    for (i = 0; i < N; i++) {
        sem_deotroy(&shm->sem[i]);
    }
    sem_destroy(&shm->mutex);
    shmdt(shm);
    shmctl(shmid, IPC_RMID, 0);

    return 0;
}