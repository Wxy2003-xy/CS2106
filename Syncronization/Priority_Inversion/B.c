#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
typedef struct {
    sem_t P;
} Mutexes;

int main(int argc, char* argv[]) {
    int shmid = atoi(argv[1]);
    Mutexes* shm = (Mutexes*) shmat(shmid, NULL, 0);
    sem_wait(&shm->P);
    int i = atoi(argv[2]);
    printf("\t[Process B] %d\n", i);
    sem_post(&shm->P);
}