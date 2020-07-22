#include "heap.h"
void heap_init(Heap *heap, int _size) {
    heap->size = 0;
    heap->node = (Process**)malloc(sizeof(Process*) * (_size + 1));
    assert(heap->node != NULL && "malloc failed.");
}

int _smaller(Process *a, Process *b) {
    if (a->remaining_time < b->remaining_time) return 1;
    if (a->remaining_time == b->remaining_time && a->index < b->index) return 1;
    return 0;
}

void _min_heapify(Heap *heap, int id) {
    int left = 2 * id, right = 2 * id + 1, smallest = id;
    if (left <= heap->size && _smaller(heap->node[left], heap->node[smallest]))
        smallest = left;
    if (right <= heap->size && _smaller(heap->node[right], heap->node[smallest]))
        smallest = right;
    if (smallest == id) return;
    swap(&heap->node[id], &heap->node[smallest]);
    _min_heapify(heap, smallest);
}

int heap_empty(Heap *heap) {
    return heap->size? 0: 1;
}

void heap_insert(Heap *heap, Process *p) {
    ++(heap->size);
    heap->node[heap->size] = p;
    int cur = heap->size;
    while (cur != 1 && _smaller(heap->node[cur], heap->node[cur / 2])) {
        swap(&heap->node[cur], &heap->node[cur / 2]);
        cur /= 2;
    }
}

Process* heap_top(Heap *heap) {
    return heap->size? heap->node[1]: NULL;
}

void heap_pop(Heap *heap) {
    swap(&heap->node[1], &heap->node[heap->size]);
    --(heap->size);
    _min_heapify(heap, 1);
}