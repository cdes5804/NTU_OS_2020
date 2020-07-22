#ifndef HEAP
#define HEAP

#include "utils.h"

typedef struct Heap {
    int size;
    Process **node;
} Heap;
void heap_init(Heap *heap, int _size);
void _min_heapify(Heap *heap, int id);
int heap_empty(Heap *heap);
void heap_insert(Heap *heap, Process *p);
Process* heap_top(Heap *heap);
void heap_pop(Heap *heap);
int _smaller(Process *a, Process *b);

#endif

