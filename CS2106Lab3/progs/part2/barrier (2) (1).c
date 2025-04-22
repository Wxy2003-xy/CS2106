#include <stdio.h>
#include <semaphore.h>
#include <sys/shm.h>

int nproc, shmid, shmid2, shmid3, shmid4;
int *count;
sem_t *barrier, *mutex, *barrier2;

void init_barrier(int numproc) {
  nproc = numproc;

  shmid = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
  shmid2 = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
  shmid3 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
  shmid4 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
  barrier = (sem_t *) shmat(shmid, NULL, 0);
  mutex = (sem_t *) shmat(shmid2, NULL, 0);
  count = (int *) shmat(shmid3, NULL, 0);
  barrier2 = (sem_t *) shmat(shmid4, NULL, 0);

  sem_init(barrier, 1, 0);
  sem_init(barrier2, 1, 1);
  sem_init(mutex, 1, 1);
  *count = 0;
}

void reach_barrier() {
  sem_wait(mutex);
  (*count)++;
  if (*count == nproc) {
    sem_wait(barrier2);
    sem_post(barrier);
  }
  sem_post(mutex);

  sem_wait(barrier);
  sem_post(barrier);

  sem_wait(mutex);
  (*count)--;
  if (*count == 0) {
    sem_wait(barrier);
    sem_post(barrier2);
  }
  sem_post(mutex);

  sem_wait(barrier2);
  sem_post(barrier2);
}

void destroy_barrier(int my_pid) {
    if(my_pid != 0) {
        // Destroy the semaphores and detach
        // and free any shared memory. Notice
        // that we explicity check that it is
        // the parent doing it.
        sem_destroy(barrier);
        sem_destroy(barrier2);
        sem_destroy(mutex);
        shmdt(barrier);
        shmdt(barrier2);
        shmdt(mutex);
        shmdt(count);
        shmctl(shmid, IPC_RMID, 0);
        shmctl(shmid2, IPC_RMID, 0);
        shmctl(shmid3, IPC_RMID, 0);
        shmctl(shmid4, IPC_RMID, 0);
    }
}


