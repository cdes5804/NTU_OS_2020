#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/timer.h>

asmlinkage void sys_get_current_time(struct timespec *t) {
    getnstimeofday(t);
}