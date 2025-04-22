#ifndef LIMIT_SEAT_SHARED
#define LIMIT_SEAT_SHARED

#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>

#define N 5
#define LEFT ((i + N - 1) % N)
#define RIGHT ((i + 1) % N)

typedef struct {
    sem_t seat;
    sem_t sem[N];
} LMTSharedMem;

#endif