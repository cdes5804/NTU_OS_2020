#include "queue.h"

void queue_init(Queue *queue) {
    queue->size = 0;
    queue->head = queue->tail = NULL;
}

int queue_empty(Queue *queue) {
    return queue->size? 0: 1;
}

void queue_push(Queue *queue, Process *p) {
    ++(queue->size);
    Node *n = (Node*)malloc(sizeof(Node));
    n->p = p;
    n->next = NULL;
    if (!queue->head) {
        queue->head = queue->tail = n;
    } else {
        queue->tail->next = n;
        queue->tail = queue->tail->next;
    }
}

Process* queue_head(Queue *queue) {
    return queue->head->p;
}

void queue_pop(Queue *queue) {
    --(queue->size);
    Node *n = queue->head;
    queue->head = queue->head->next;
    free(n);
}