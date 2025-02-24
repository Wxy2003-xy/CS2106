#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>

/**
 * Compile and run master_prog.c first;
 * Get the shmid,
 * On another terminal, compile and run slave_prog.c, 
 * Enter the shmid from shmget when running master_prog, and params
 * slave_prog executes
 * master_prog get the result back from slave_prog after its execution
*/


int main() {
    int shmid;
    int i;
    int* shm;
    printf("Master prog starts...\n");
    fflush(stdout);
    
    shmid = shmget( IPC_PRIVATE, 4*sizeof(int), 0644 | IPC_CREAT);
    printf("Finish allocation:\n");
    printf("%d\n", shmid);
    if (shmid == -1) {
        perror("shmget failed\n");
        exit(1);
    } else {
        printf("Shared mem created\n");
    } 

    shm = (int*) shmat( shmid, NULL, 0 ); 
    if (shm == (int*) -1) { 
        printf("cannot attach mem block\n ");
        exit(1);
    }

    shm[0] = 0;             
    while(shm[0] == 0) {
        sleep(3);
    }

    for (i = 0; i < 3; i++) {
        printf("read %d from shared memory\n", shm[i+1]);
    }

    shmdt( (char*) shm );
    shmctl(shmid, IPC_RMID, 0);
    return 0;
}
