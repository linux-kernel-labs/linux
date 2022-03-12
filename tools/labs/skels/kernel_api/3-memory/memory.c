/*
 * SO2 lab3 - task 3
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

MODULE_DESCRIPTION("Memory processing");
MODULE_AUTHOR("SO2");
MODULE_LICENSE("GPL");

struct task_info {
	pid_t pid;
	unsigned long timestamp;
};

static struct task_info *ti1, *ti2, *ti3, *ti4;

static struct task_info *task_info_alloc(int pid)
{
	struct task_info *ti;

	/* TODO 1: allocated and initialize a task_info struct */

	return ti;
}

static int memory_init(void)
{
	/* TODO 2: call task_info_alloc for current pid */

	/* TODO 2: call task_info_alloc for parent PID */

	/* TODO 2: call task_info alloc for next process PID */

	/* TODO 2: call task_info_alloc for next process of the next process */

	return 0;
}

static void memory_exit(void)
{

	/* TODO 3: print ti* field values */

	/* TODO 4: free ti* structures */
}

module_init(memory_init);
module_exit(memory_exit);
