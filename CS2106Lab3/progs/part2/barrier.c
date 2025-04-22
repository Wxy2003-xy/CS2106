#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
int *_count = 0;
int _np;
sem_t* _semaphores;
sem_t* _sem_count;
int _shmid_sems;
int _shmid_count;
int _shmid_count_sem;
void init_barrier(int numproc) {
    int i;
    _np = numproc;
    *_count = 0;
    _shmid_sems = shmget(IPC_PRIVATE, numproc*sizeof(sem_t), IPC_CREAT | 0600);
    _semaphores = (sem_t*) shmat(_shmid_sems, NULL, 0);

    _shmid_count = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    _count = (int*) shmat(_shmid_count, NULL, 0);

    _shmid_count_sem = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
    _sem_count = (sem_t*) shmat(_shmid_count_sem, NULL, 0);
    sem_init(_semaphores, 1, 0);
    for (i = 1; i < numproc; i++) {
        sem_init(&_semaphores[i], 1, 0);
    }
    sem_init(_sem_count, 1, 1);
}

void reach_barrier() {
    sem_wait(_sem_count);
    *_count += 1;
    if (*_count == _np) {
        int j;
        for (j = 0; j < _np; j++) {
            sem_post(&_semaphores[j]);
        }
        *_count = 0;
        sem_post(_sem_count);
    } else {
        sem_post(_sem_count);
        sem_wait(&_semaphores[*_count]);
    }
}

void destroy_barrier(int my_pid) {
    if(my_pid != 0) {
        // Destroy the semaphores and detach
        // and free any shared memory. Notice
        // that we explicity check that it is
        // the parent doing it.
        int k;
        for (k = 0; k < _np; k++) {
            sem_destroy(&_semaphores[k]);
        }
        shmdt(_semaphores);
        sem_destroy(_sem_count);
        shmdt(_sem_count);
        shmdt(_count);
        shmctl(_shmid_sems, IPC_RMID, 0);
        shmctl(_shmid_count, IPC_RMID, 0);
        shmctl(_shmid_count_sem, IPC_RMID, 0);

    }
}


