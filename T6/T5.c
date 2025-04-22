#include <stdio.h>
#include <stdbool.h>
bool _sync_bool_compare_and_swap(int* t, int v, int n) {
    if (*t == v) {
        *t = n;
        return 1;
    } else {
        return 0;
    }
}
int atomic_increment(int* t) {
    int temp;
    do {
        temp = *t;
    } while (!_sync_bool_compare_and_swap(t, temp, temp + 1));
    return temp + 1;
}

int main() {
    return 0;
}