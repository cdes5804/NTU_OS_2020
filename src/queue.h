#ifndef QUEUE
#define QUEUE

#include "utils.h"
typedef struct Node {
    Process *p;
    struct Node *next;
} Node;
typedef struct Queue {
    Node *head, *tail;
    int size;
} Queue;


void queue_init(Queue *queue);
int queue_empty(Queue *queue);
void queue_push(Queue *queue, Process *p);
Process* queue_head(Queue *queue);
void queue_pop(Queue *queue);

#endif
