#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

void* roll() {
    int* value = malloc(sizeof(int));
    if (value == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }
    *value = (rand() % 6) + 1;
    printf("%d\n", *value);
    fflush(stdout);  // Ensure immediate output
    return (void*) value;
}

int main(int argc, char* argv[]) {
    int* res;
    srand(time(NULL));
    pthread_t th;

    printf("start\n");
    fflush(stdout);  // Ensure immediate output

    if (pthread_create(&th, NULL, roll, NULL) != 0) {
        perror("Thread creation failed");
        return 1;
    }

    if (pthread_join(th, (void**) &res) != 0) {
        perror("Thread join failed");
        return 2;
    }

    if (res == NULL) {
        printf("Thread returned NULL\n");
        return 3;
    }

    printf("%d\n", *res);
    free(res);  // Free allocated memory
    return 0;
}
