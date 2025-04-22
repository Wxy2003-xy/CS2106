#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"  
#include "llist.h"     
char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL;
TNode *_alloclist = NULL;

long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long)((char *)ptr - &_heap[0]);
}
void print_alloc_node(TNode *node) {
    printf("Status: ALLOCATED Start index: %u Length: %d\n",
        node->key,
        node->pdata->size);
}
void print_free_node(TNode *node) {
    printf("Status: FREE Start index: %u Length: %d\n",
        node->key,
        node->pdata->size);
}

void print_memlist() {
    process_list(_alloclist, print_alloc_node);
    process_list(_memlist, print_free_node);
}
static TData *make_data(int size, int alloc_flag) {
    TData *d = malloc(sizeof(TData));
    d->size  = size;
    d->alloc = alloc_flag;  // 1 for allocated blocks, 0 for free blocks
    return d;
}

TData *create_data(int size) {
    TData *data = malloc(sizeof(TData));
    data->size = size; 
    return data;
}
void init_memory_manager(void) {
    if (_memlist == NULL) {
        // Entire heap is one big free block
        TData *d = make_data(MEMSIZE, 0);
        _memlist = make_node(0, d);
    }
}
void *mymalloc(size_t size) {
    if (size == 0) return NULL;
    if (_memlist == NULL) init_memory_manager();

    // First‑fit search
    TNode *cur = _memlist;
    while (cur && cur->pdata->size < (int)size)
        cur = cur->next;
    if (!cur) return NULL; // no suitable block

    unsigned int idx = cur->key;
    int        sz  = (int)size;

    // Carve out from free block
    if (cur->pdata->size == sz) {
        delete_node(&_memlist, cur);
    } else {
        cur->key           += sz;
        cur->pdata->size   -= sz;
    }

    // Record in _alloclist
    TData *d = make_data(sz, 1);
    TNode *n = make_node(idx, d);
    insert_node(&_alloclist, n, ASCENDING);

    return &_heap[idx];
}

void merge_free_list(void) {
    TNode *cur = _memlist;
    while (cur && cur->next) {
        if (cur->key + cur->pdata->size == cur->next->key) {
            cur->pdata->size += cur->next->pdata->size;
            delete_node(&_memlist, cur->next);
            continue;
        }
        cur = cur->next;
    }
}
void myfree(void *ptr) {
    if (ptr == NULL)
        return;
    unsigned int freed_index = (unsigned int)get_index(ptr);
    TNode *allocNode = find_node(_alloclist, freed_index);
    if (allocNode == NULL) {
        return;
    }
    int blockSize = allocNode->pdata->size;
    delete_node(&_alloclist, allocNode);
    TData *freeData = create_data(blockSize);
    TNode *freeNode = make_node(freed_index, freeData);
    insert_node(&_memlist, freeNode, ASCENDING);
    merge_free_list();
}
