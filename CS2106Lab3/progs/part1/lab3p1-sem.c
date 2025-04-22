#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#define NUM_PROCESSES 5

int main() {

    int i, j, pid, shmid, x;
    sem_t *sem;
    x = -1;
    shmid = shmget(IPC_PRIVATE, 
        NUM_PROCESSES * sizeof(sem_t), IPC_CREAT | 0600);
    sem = (sem_t *) shmat(shmid, NULL, 0);
    sem_init(sem, 1, 1);
    for (i=1; i<NUM_PROCESSES; i++) {
        sem_init(&sem[i], 1, 0);
    }
    for(i=0; i<NUM_PROCESSES; i++)
    {
        if((pid = fork()) == 0) {
            x = i;
            break;
        }
    }

    if(pid == 0) {
        sem_wait(&sem[x]);
        printf("I am child %d\n", i);

        for(j = i*10; j<i*10 + 10; j++){
            printf("%d ", j);
            fflush(stdout);
            usleep(2500);
        }   

        printf("\n\n");
        if (x + 1 < NUM_PROCESSES) {
            sem_post(&sem[x+1]);
        }
    }
    else {
        for(i=0; i<NUM_PROCESSES; i++) {
            wait(NULL);
        }
        for (i=0; i < NUM_PROCESSES; i++) {
            sem_destroy(&sem[i]);
        }
        shmdt(sem);
        shmctl(shmid, IPC_RMID, 0);
    }
}

