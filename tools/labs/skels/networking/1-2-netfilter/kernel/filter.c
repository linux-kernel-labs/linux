/*
 * SO2 - Networking Lab (#10)
 *
 * Exercise #1, #2: simple netfilter module
 *
 * Code skeleton.
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/atomic.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#include "filter.h"

MODULE_DESCRIPTION("Simple netfilter module");
MODULE_AUTHOR("SO2");
MODULE_LICENSE("GPL");

#define LOG_LEVEL		KERN_ALERT
#define MY_DEVICE		"filter"

static struct cdev my_cdev;
static atomic_t ioctl_set;
static unsigned int ioctl_set_addr;


/* Test ioctl_set_addr if it has been set.
 */
static int test_daddr(unsigned int dst_addr)
{
	int ret = 0;

	/* TODO 2: return non-zero if address has been set
	 * *and* matches dst_addr
	 */

	return ret;
}

/* TODO 1: netfilter hook function */

static int my_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
	return 0;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case MY_IOCTL_FILTER_ADDRESS:
		/* TODO 2: set filter address from arg */
		break;

	default:
		return -ENOTTY;
	}

	return 0;
}

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	.unlocked_ioctl = my_ioctl
};

/* TODO 1: define netfilter hook operations structure */

int __init my_hook_init(void)
{
	int err;

	/* register filter device */
	err = register_chrdev_region(MKDEV(MY_MAJOR, 0), 1, MY_DEVICE);
	if (err != 0)
		return err;

	atomic_set(&ioctl_set, 0);
	ioctl_set_addr = 0;

	/* init & add device */
	cdev_init(&my_cdev, &my_fops);
	cdev_add(&my_cdev, MKDEV(MY_MAJOR, 0), 1);

	/* TODO 1: register netfilter hook */

	return 0;

out:
	/* cleanup */
	cdev_del(&my_cdev);
	unregister_chrdev_region(MKDEV(MY_MAJOR, 0), 1);

	return err;
}

void __exit my_hook_exit(void)
{
	/* TODO 1: unregister hook */

	/* cleanup device */
	cdev_del(&my_cdev);
	unregister_chrdev_region(MKDEV(MY_MAJOR, 0), 1);
}

module_init(my_hook_init);
module_exit(my_hook_exit);
