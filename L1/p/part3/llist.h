
#define MAX_FNAME_LEN   64

typedef struct llist {
    char filename[MAX_FNAME_LEN];
    int filesize;
    int startblock;
    struct llist *prev, *next;
} TLinkedList;

// Fill in the prototypes below
void init_llist(TLinkedList**);
TLinkedList *create_node(char*, int, int);
void insert_llist(TLinkedList**, TLinkedList*);
void delete_llist(TLinkedList**, TLinkedList*);
TLinkedList* find_llist(TLinkedList*, char*);
void traverse(TLinkedList**, void(*fn)(TLinkedList*));

