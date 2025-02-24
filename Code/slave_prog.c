#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
int main() {
    int shmid;
    int i;
    int* shm;
    int input;

    printf("Shared mem if for attachment: \n");
    scanf("%d", &shmid);
    shm = (int*) shmat(shmid, NULL, 0);
    if (shm == (int*) -1) {
        printf("cannot attach mem block ");
        exit(1);
    }
    for (i = 0; i < 3; i++) {
        scanf("%d", &input);
        shm[i+1] = input;
    }
    shm[0] = 1;         // signals the slave_prog has finished
    shmdt( (char*) shm );
    return 0;
}