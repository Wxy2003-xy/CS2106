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
    sem_t Q;
} Mutexes;

int main() {
    int i, shmid;
    shmid = shmget(IPC_PRIVATE, sizeof(Mutexes), IPC_CREAT | 0600);
    Mutexes* shm = (Mutexes*) shmat(shmid, NULL, 0);

    sem_init(&shm->P, 1, 1);
    char shmid_str[20];
    sprintf(shmid_str, "%d", shmid);

    for (i = 0; i < 10; i++) {
        char iter[8];
        sprintf(iter, "%d", i);
        char* arga[] = {"./A", shmid_str, iter, NULL};
        char* argb[] = {"./B", shmid_str, iter, NULL};
        if (fork() == 0) {
            execvp(arga[0], arga);
            exit(1);
        }
        if (fork() == 0) {
            execvp(argb[0], argb);
            exit(1);
        }
    }
    for (i = 0; i < 20; i++) {
        wait(NULL);
    }
    shmdt(shm);
    shmctl(shmid, IPC_RMID, 0);
    return 0;

}