#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_DESCRIPTION("Simple module");
MODULE_AUTHOR("Kernel Hacker");
MODULE_LICENSE("GPL");

static int my_hello_init(void)
{
	printk(KERN_ALERT "Hello!\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye!\n");
}

module_init(my_hello_init);
module_exit(hello_exit);
