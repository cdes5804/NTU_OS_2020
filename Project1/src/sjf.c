#include "sjf.h"
void SJF_SCH(int n, Process *process) {
    Heap heap;
    heap_init(&heap, n);
    int current = 0, end_time = -1, running = -1;
    for (int current_time = 0; ; ++current_time) {
        while (current < n && process[current].ready_time <= current_time) {
            heap_insert(&heap, &process[current]);
            ++current;
        }
        if (!heap_empty(&heap) && running == -1) {
            Process *p = heap_top(&heap);
            heap_pop(&heap);
            run_process(p);
            end_time = current_time + p->execution_time;
            running = p->index;
        }
        if (end_time == current_time) {
            waitpid(process[running].pid, NULL, 0);
            end_time = running = -1;
        }
        if (current == n && heap_empty(&heap) && running == -1) break;
        unit_time();
    }
}