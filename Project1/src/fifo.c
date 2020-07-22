#include "fifo.h"
void FIFO_SCH(int n, Process *process) {
    int current = 0, end_time = -1, running = -1;
    for (int current_time = 0; ; ++current_time) {
        if (process[current].ready_time <= current_time && running == -1) {
            run_process(&process[current]);
            end_time = current_time + process[current].execution_time;
            running = process[current].index;
        }
        if (end_time == current_time) {
            waitpid(process[running].pid, NULL, 0);
            end_time = running = -1;
            ++current;
        }
        if (current == n && running == -1) break;
        unit_time();
    }
}
