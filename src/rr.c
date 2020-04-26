#include "rr.h"
void RR_SCH(int n, Process *process) {
    Queue queue;
    queue_init(&queue);
    int current = 0, counter = -1, running = -1;
    for (int current_time = 0; ; ++current_time) {
        while (current < n && process[current].ready_time <= current_time) {
            queue_push(&queue, &process[current]);
            ++current;
        }
        if (counter == 0) {
            set_priority(process[running].pid, 1);
            queue_pop(&queue);
            if (process[running].remaining_time) queue_push(&queue, &process[running]);
            else waitpid(process[running].pid, NULL, 0);
            running = -1;
        }
        if (current == n && queue_empty(&queue) && running == -1) break;
        if (!queue_empty(&queue) && running == -1) {
            Process *p = queue_head(&queue);
            if (!p->running) run_process(p);
            else set_priority(p->pid, 99);
            counter = min(p->remaining_time, quantum);
            p->remaining_time -= counter;
            running = p->index;
        }
        if (counter > 0) --counter;
        unit_time();
    }
}