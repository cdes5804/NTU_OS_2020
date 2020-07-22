#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/timer.h>

asmlinkage void sys_print_info(pid_t pid, time_t t1, long t2, time_t t3, long t4) {
    printk(KERN_INFO "[OS-project1] %d %ld.%09ld %ld.%09ld\n", (int)pid, (long)t1, t2, (long)t3, t4);
}
