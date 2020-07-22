#include "utils.h"

void unit_time(void) {
    volatile unsigned long i;
    for (i = 0; i < 1000000UL; ++i);
}

int cmp(const void *a, const void *b) {
    Process *pa = (Process*)a, *pb = (Process*)b;
    return pa->ready_time > pb->ready_time? 1: -1;
}

int set_cpu(pid_t pid, int cpu_no) {
    assert(cpu_no < CPU_SETSIZE && "Not enough CPU to assign.");
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_no, &cpu);
    assert(sched_setaffinity(pid, sizeof(cpu), &cpu) == 0 && "Failed to set affinity.");
    return 0;
}

int set_priority(pid_t pid, int priority) {
    struct sched_param param;
	param.sched_priority = priority;
    assert(sched_setscheduler(pid, SCHED_FIFO, &param) == 0 && "Unable to set priority.");
    return 0;
}

int run_process(Process *p) {
    pid_t pid = fork();
    assert(pid >= 0 && "Fork Error.");
    if (pid == 0) {
        p->pid = getpid();
        struct timespec start, end;
        printf("%s %d\n", p->name, p->pid);
        fflush(stdout);
        syscall(333, &start);
        for (int i = 0; i < p->execution_time; ++i) unit_time();
        syscall(333, &end);
        syscall(334, p->pid, start.tv_sec, start.tv_nsec, end.tv_sec, end.tv_nsec);
        exit(0);
    } else {
        p->pid = pid;
        p->running = 1;
        assert(set_cpu(pid, 1) == 0 && "unable to set CPU.");
        assert(set_priority(pid, 99) == 0 && "unable to set priority.");
        return 0;
    }
}

void swap(Process **a, Process **b) {
    Process *c = *a;
    *a = *b;
    *b = c;
}

int min(int a, int b) {
    return a < b? a: b;
}