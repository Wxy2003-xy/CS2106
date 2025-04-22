#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "share.h"

void consume(int input, int* buffer) {
    sleep(10);
    int i;
    printf("Buffer: [ ");
    for (i = 0; i < BUFFER_SIZE; i++) {
        printf("%d ", buffer[i]);
    }
    printf("] consumed: %d\n", input);
}

int main(int argc, char* argv[]) {
    int shmid = atoi(argv[1]);
    SharedMem* shm = (SharedMem*) shmat(shmid, NULL, 0);
    int cont = 1;

    // while (cont) {
        sem_wait(&shm->notEmpty);
        sem_wait(&shm->mutex);
        int output = shm->buffer[shm->out_idx];
        shm->buffer[shm->out_idx] = 0;
        shm->out_idx = (shm->out_idx + 1) % BUFFER_SIZE;
        shm->count--;
        consume(output, shm->buffer);
        sem_post(&shm->mutex);
        sem_post(&shm->notFull);
        cont = 0;
    // }
    return 0;
}