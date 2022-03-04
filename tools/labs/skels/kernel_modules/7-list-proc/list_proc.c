#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

MODULE_DESCRIPTION("List current processes");
MODULE_AUTHOR("Kernel Hacker");
MODULE_LICENSE("GPL");

static int my_proc_init(void)
{
	struct task_struct *p;

	p = get_current();

	/* Print current process pid and its name */
	pr_info("pid : %d; name: %s\n", p->pid, p->comm);

	/* Print the pid and name of all processes */
	// https://stackoverflow.com/questions/18408766/make-a-system-call-to-get-list-of-processes
    	for_each_process(p)
		pr_info("pid : %d; name: %s\n", p->pid, p->comm);

	return 0;
}

static void my_proc_exit(void)
{
	/* Print current process pid and name */
	struct task_struct *p;

	p = get_current();
	pr_info("pid : %d; name: %s\n", p->pid, p->comm);
}

module_init(my_proc_init);
module_exit(my_proc_exit);
