#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL;
TNode *_alloc_history = NULL; // Tracks original allocated blocks

long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long)((char *)ptr - &_heap[0]);
}

long get_size(void *ptr) {
    if(ptr == NULL)
        return -1;
    unsigned int key = (unsigned int)get_index(ptr);
    TNode *node = find_node(_memlist, key);
    if(!node)
        return -1;
    return (long)((TData*)node->pdata)->size;
}

static TNode *create_block(unsigned int key, size_t size, int free_flag) {
    TData *data = malloc(sizeof(TData));
    if(!data) {
        fprintf(stderr, "Error allocating TData\n");
        exit(EXIT_FAILURE);
    }
    data->size = size;
    data->free = free_flag;
    return make_node(key, data);
}

static TNode *find_free_block(size_t req_size) {
    TNode *current = _memlist;
    TNode *best_fit = NULL;
    
    while(current) {
        if(((TData*)current->pdata)->free && ((TData*)current->pdata)->size >= req_size) {
            if (!best_fit || ((TData*)current->pdata)->size < ((TData*)best_fit->pdata)->size) {
                best_fit = current;
                
                // If we found an exact match, use it immediately
                if (((TData*)current->pdata)->size == req_size)
                    return current;
            }
        }
        current = current->next;
    }
    
    return best_fit;  // Return the smallest block that fits our needs
}

// Split the block until it's the appropriate size for the request
static void split_block(TNode *node, size_t target_size) {
    size_t current_size = ((TData*)node->pdata)->size;
    
    while(current_size > target_size) {
        size_t new_size = current_size / 2;
        
        // Update current node (left buddy)
        ((TData*)node->pdata)->size = new_size;
        
        // Create right buddy
        TNode *buddy = create_block(node->key + new_size, new_size, 1);
        insert_node(&_memlist, buddy, ASCENDING);
        
        // Save split buddy to history for printing
        TNode *hist_right = create_block(buddy->key, buddy->pdata->size, buddy->pdata->free);
        insert_node(&_alloc_history, hist_right, ASCENDING);
        
        current_size = new_size;
    }
}

static void merge_block(TNode *node) {
    int merged;
    do {
        merged = 0;
        size_t size = ((TData*)node->pdata)->size;
        unsigned int offset = node->key;

        unsigned int buddy_offset = offset ^ size;
        TNode *buddy = find_node(_memlist, buddy_offset);

        if(buddy && ((TData*)buddy->pdata)->free && ((TData*)buddy->pdata)->size == size) {
            unsigned int merged_key = (offset < buddy_offset) ? offset : buddy_offset;

            TNode *merged_node = create_block(merged_key, size * 2, 1);

            delete_node(&_memlist, node);
            delete_node(&_memlist, buddy);

            insert_node(&_memlist, merged_node, ASCENDING);

            node = merged_node;
            merged = 1;
        }
    } while(merged);
}

void *mymalloc(size_t size) {
    if(size == 0)
        return NULL;

    if(_memlist == NULL) {
        TNode *initial = create_block(0, MEMSIZE, 1);
        _memlist = initial;

        // Track the original full block
        TNode *hist = create_block(initial->key, initial->pdata->size, initial->pdata->free);
        _alloc_history = hist;
    }

    size_t req_size = (size < MINIMUM_BLOCK_SIZE) ? MINIMUM_BLOCK_SIZE : size;
    size_t block_size = MINIMUM_BLOCK_SIZE;
    while(block_size < req_size)
        block_size *= 2;

    if(block_size > MEMSIZE)
        return NULL;

    TNode *block = find_free_block(block_size);
    if(block == NULL)
        return NULL;

    if(((TData*)block->pdata)->size > block_size) {
        // Save this original block to history before splitting
        TNode *hist = create_block(block->key, block->pdata->size, block->pdata->free);
        insert_node(&_alloc_history, hist, ASCENDING);
        split_block(block, block_size);
    }

    // The node may have changed after splitting, so find it again if needed
    if(((TData*)block->pdata)->size != block_size) {
        block = find_free_block(block_size);
        if(block == NULL)
            return NULL;
    }

    ((TData*)block->pdata)->free = 0;

    // Save allocated block to history
    TNode *hist = create_block(block->key, block->pdata->size, 0);
    insert_node(&_alloc_history, hist, ASCENDING);

    return (void *)(&_heap[block->key]);
}

void myfree(void *ptr) {
    if(ptr == NULL)
        return;

    unsigned int offset = (unsigned int)get_index(ptr);
    TNode *block = find_node(_memlist, offset);
    if(block == NULL)
        return;

    ((TData*)block->pdata)->free = 1;
    merge_block(block);

    // Also update history if it exists
    TNode *hist = find_node(_alloc_history, offset);
    if(hist)
        ((TData*)hist->pdata)->free = 1;
}

void print_memlist() {
    for (int order = MAX_ORDER - 1; order >= 0; order--) {
        size_t block_size = (size_t)1 << (LOG_MINIMUM_BLOCK + order);
        size_t block_size_kb = block_size / 1024;
        printf("Block size %zu KB:", block_size_kb);

        int first_printed = 1;  // flag to manage arrow printing

        // Process each region of size 'block_size' in ascending order.
        for (size_t region_offset = 0; region_offset < MEMSIZE; region_offset += block_size) {
            int region_split = 0;
            // Check if this region contains an allocated block strictly smaller than block_size.
            for (TNode *n = _memlist; n; n = n->next) {
                size_t start = n->key;
                size_t size  = ((TData*)n->pdata)->size;
                size_t end   = start + size;
                if (!((TData*)n->pdata)->free &&
                    size < block_size &&
                    start >= region_offset && end <= region_offset + block_size) {
                    region_split = 1;
                    break;
                }
            }
            // If a split exists in this region, print a region-level header using this region's offset.
            if (region_split) {
                if (!first_printed)
                    printf(" -> ");
                else {
                    printf(" ");
                    first_printed = 0;
                }
                printf("ALLOCATED, %zu, %zu", region_offset / 1024, block_size_kb);
            }

            // Collect nodes that have exactly 'block_size' in this region.
            TNode *nodes[256];  // adjust as needed
            int count = 0;
            for (TNode *n = _memlist; n; n = n->next) {
                if (((TData*)n->pdata)->size == block_size &&
                    n->key >= region_offset && n->key < region_offset + block_size) {
                    nodes[count++] = n;
                }
            }
            // Sort these nodes by their starting offset.
            for (int i = 0; i < count - 1; i++) {
                for (int j = i + 1; j < count; j++) {
                    if (nodes[i]->key > nodes[j]->key) {
                        TNode *tmp = nodes[i];
                        nodes[i] = nodes[j];
                        nodes[j] = tmp;
                    }
                }
            }
            // Print the sorted node entries.
            for (int i = 0; i < count; i++) {
                if (!first_printed)
                    printf(" -> ");
                else {
                    printf(" ");
                    first_printed = 0;
                }
                unsigned int off_kb = nodes[i]->key / 1024;
                size_t size_kb = ((TData*)nodes[i]->pdata)->size / 1024;
                if (((TData*)nodes[i]->pdata)->free)
                    printf("FREE, %u, %zu", off_kb, size_kb);
                else
                    printf("ALLOCATED, %u, %zu", off_kb, size_kb);
            }
        }
        if (!first_printed)
            printf(" ->");
        printf("\n");
    }
}