#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"
#include "bitmap.h"

char _heap[MEMSIZE] = {0};
unsigned char _bitmap[MEMSIZE / 8] = {0};
typedef struct {
    void  *ptr;   
    size_t size;  
} alloc_record_t;
static alloc_record_t allocations[MEMSIZE] = { {0,0} };
long get_index(void *ptr) {
    if (ptr == NULL) return -1;
    return (long)((char*)ptr - &_heap[0]);
}
void print_memlist() {
    print_map(_bitmap, MEMSIZE / 8);
}
void *mymalloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    long idx = search_map(_bitmap, MEMSIZE / 8, size);
    if (idx < 0) {
        
        return NULL;
    }
    allocate_map(_bitmap, idx, size);
    void *userPtr = &_heap[idx];
    int i;
    for (i = 0; i < MEMSIZE; i++) {
        if (allocations[i].ptr == NULL) {
            allocations[i].ptr  = userPtr;
            allocations[i].size = size;
            break;
        }
    }
    return userPtr;
}
void myfree(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    int i;
    for (i = 0; i < MEMSIZE; i++) {
        if (allocations[i].ptr == ptr) {
            size_t size = allocations[i].size;
            long offset = get_index(ptr);
            if (offset >= 0) {
                free_map(_bitmap, offset, size);
            }
            allocations[i].ptr  = NULL;
            allocations[i].size = 0;

            return; 
        }
    }
}
