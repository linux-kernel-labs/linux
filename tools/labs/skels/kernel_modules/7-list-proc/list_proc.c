#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>/* TODO: add missing headers */
#include <linux/sched/signal.h>
MODULE_DESCRIPTION("List current processes");
MODULE_AUTHOR("Kernel Hacker");
MODULE_LICENSE("GPL");

static int my_proc_init(void)
{
	struct task_struct *p = current;
        /* TODO: print current process pid and its name */
  	pr_info("CURRENT-> Process:%s  PID:[%d]\n",p->comm, p->pid);
	/* TODO: print the pid and name of all processes */
        pr_info("Current Running Processes\n");
	struct task_struct *task_list;
	for_each_process(task_list) 
	{
		pr_info("Process:%s  PID:[%d]\n",task_list->comm, task_list->pid); 
	}
	return 0;
}

static void my_proc_exit(void)
{
        struct task_struct *p = current;
	pr_info("CURRENT-> Process:%s  PID:[%d]\n",p->comm, p->pid);/* TODO: print current process pid and name */  
}

module_init(my_proc_init);
module_exit(my_proc_exit);
