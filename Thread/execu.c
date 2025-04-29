#include <pthread.h>
#include <stdio.h>

void* t1(void* p) {
    pthread_exit((void*) 2);
}

void* t2(void* p) {
    pthread_exit((void*) 3);
}

int main() {
    pthread_t a, b;
    pthread_create(&a, NULL, t1, NULL);
    pthread_create(&b, NULL, t2, NULL);
    int c, d;
    void* e;    
    void* f;
    c = pthread_join(a, &e);
    d = pthread_join(b, &f);
    printf("%d", c + (int) f);
}