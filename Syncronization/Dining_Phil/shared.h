#ifndef SHARED_H
#define SHARED_H
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
#define THINKING 0
#define HUNGRY 1
#define EATING 2

typedef struct {
    int _status[N];
    sem_t mutex;
    sem_t sem[N]; 
} SharedMem;

#endif