#ifndef UTILS
#define UTILS

#define _GNU_SOURCE
#include <assert.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>
typedef struct Process {
    char name[50];
    pid_t pid;
    int ready_time, execution_time, remaining_time, running, index;
} Process;

void unit_time(void);
int set_cpu(pid_t pid, int cpu_no);
int set_priority(pid_t pid, int priority);
int run_process(Process *p);
int cmp(const void *a, const void *b);
void swap(Process **a, Process **b);
int min(int a, int b);

#endif