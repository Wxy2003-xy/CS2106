#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "share.h"

int compute(int input) {
    sleep(1);
    return input;
}

void produce_print(int input, int* buffer) {
    int i;
    printf("Buffer: [ ");
    for (i = 0; i < BUFFER_SIZE; i++) {
        printf("%d ", buffer[i]);
    }
    printf("] produced: %d\n", input);
}

int main(int argc, char* argv[]) {
    int shmid = atoi(argv[1]);
    SharedMem* shm = (SharedMem*) shmat(shmid, NULL, 0);
    int cont = 1;

    // while (1) {
        int output = compute(atoi(argv[2]));
        sem_wait(&shm->notFull);
        sem_wait(&shm->mutex);
        shm->buffer[shm->in_idx] = output;
        shm->in_idx = (shm->in_idx + 1) % BUFFER_SIZE;
        shm->count++;
        produce_print(output, shm->buffer);
        sem_post(&shm->mutex);
        sem_post(&shm->notEmpty);
        cont = 0;
    // }
    return 0;
}