#include "psjf.h"
void PSJF_SCH(int n, Process *process) {
    Heap heap;
    heap_init(&heap, n);
    int current = 0, running = -1;
    for (int current_time = 0; ; ++current_time) {
        Process *p = heap_top(&heap);
        if (p && !p->remaining_time) {
            waitpid(p->pid, NULL, 0);
            heap_pop(&heap);
            running = -1;
        }
        if (current == n && heap_empty(&heap) && running == -1) break;
        while (current < n && process[current].ready_time <= current_time) {
            heap_insert(&heap, &process[current]);
            ++current;
        }
        p = heap_top(&heap);
        if (p && running >= 0 && process[running].pid != p->pid && process[running].remaining_time > p->remaining_time) {
            set_priority(process[running].pid, 1);
        }
        if (p && running != p->index) {
            if (!p->running) run_process(p);
            else set_priority(p->pid, 99);
            running = p->index;
            --(p->remaining_time);
        }
        if (running >= 0) --process[running].remaining_time;
        unit_time();
    }
}