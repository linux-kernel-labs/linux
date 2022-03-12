#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
/* TODO: add missing headers */
#include <linux/sched.h>
#include <linux/sched/signal.h>


MODULE_DESCRIPTION("List current processes");
MODULE_AUTHOR("Kernel Hacker");
MODULE_LICENSE("GPL");

static int my_proc_init(void)
{
	struct task_struct *p = current;
	struct task_struct *first = current;

	/* TODO: print current process pid and its name */
	pr_info("The current proces id = %d, name = %s\n", p->pid, p->comm);
	/* TODO: print the pid and name of all processes */
	
	p = next_task(p);
	while (p != first) {
		pr_info("The current proces id = %d, name = %s\n", p->pid, p->comm);
		p = next_task(p);
	}
	return 0;
}

static void my_proc_exit(void)
{
	/* TODO: print current process pid and name */
	struct task_struct *p = current;
	pr_info("The current proces id = %d, name = %s\n", p->pid, p->comm);

}

module_init(my_proc_init);
module_exit(my_proc_exit);
