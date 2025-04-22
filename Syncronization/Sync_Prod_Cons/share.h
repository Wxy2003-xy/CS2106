#ifndef SHARE_H
#define SHARE_H

#include <semaphore.h>

#define BUFFER_SIZE 3

typedef struct {
    sem_t mutex;
    sem_t notEmpty;
    sem_t notFull;
    int in_idx, out_idx, count;
    int buffer[BUFFER_SIZE];
} SharedMem;

#endif
