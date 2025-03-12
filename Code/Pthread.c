#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

void* roll() {
    int value = (rand() % 6) + 1;
    int* res = malloc(sizeof(int));
    *res = value;
    printf("%p\n", res);
    return (void*) res;
}

int main(int argc, char* argv[]) {
    int* res;
    srand(time(NULL));
    pthread_t th;
    if (pthread_create(&th, NULL, roll, NULL) != 0) {
        return 1;
    }
    if (pthread_join(th, (void**) &res) != 0) {
        perror("Thread join failed");
        return 2;
    }
    printf("%d\n", *res);
    free(res);
    return 0;
}
