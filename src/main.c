#include "scheduler.h"
int main() {
    char policy[10]; scanf("%s", policy);
    int n; scanf("%d", &n);
    Process *process = (Process*)malloc(sizeof(Process) * n);
    memset(process, 0, sizeof(Process) * n);
    for (int i = 0; i < n; ++i) {
        scanf("%s%d%d", process[i].name, &process[i].ready_time, &process[i].execution_time);
        process[i].remaining_time = process[i].execution_time;
    }
    qsort(process, n, sizeof(Process), cmp);
    for (int i = 0; i < n; ++i) process[i].index = i;
    assert(set_cpu(getpid(), 0) == 0 && "unable to set CPU.");
    assert(set_priority(getpid(), 99) == 0 && "unable to set priority.");

    if (strcmp(policy, "FIFO") == 0) FIFO_SCH(n, process);
    else if (strcmp(policy, "RR") == 0) RR_SCH(n, process);
    else if (strcmp(policy, "SJF") == 0) SJF_SCH(n, process);
    else if (strcmp(policy, "PSJF") == 0) PSJF_SCH(n, process);
    else {
        fprintf(stderr, "Unknown type %s. Quittings..\n", policy);
        exit(1);
    }
    exit(0);
}