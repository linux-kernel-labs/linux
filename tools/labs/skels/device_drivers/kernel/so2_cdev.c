/*
 * Character device drivers lab
 *
 * All tasks
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/wait.h>

#include "../include/so2_cdev.h"

MODULE_DESCRIPTION("SO2 character device");
MODULE_AUTHOR("SO2");
MODULE_LICENSE("GPL");

#define LOG_LEVEL	KERN_INFO

#define MY_MAJOR		42
#define MY_MINOR		0
#define NUM_MINORS		1
#define MODULE_NAME		"so2_cdev"
#define MESSAGE			"hello\n"
#define IOCTL_MESSAGE		"Hello ioctl"

#ifndef BUFSIZ
#define BUFSIZ		4096
#endif



struct so2_device_data {
	/* TODO 2: add cdev member */
	struct cdev cdev;
	/* TODO 4: add buffer with BUFSIZ elements */
	char* read_buffer[BUFSIZ];
	/* TODO 7: extra members for home */
	/* TODO 3: add atomic_t access variable to keep track if file is opened */
	atomic_t dump_lock;
};

struct so2_device_data devs[NUM_MINORS];

static int so2_cdev_open(struct inode *inode, struct file *file)
{
	struct so2_device_data *data;

	/* TODO 2: print message when the device file is open. */
	pr_info("Device file is open!\n");
	/* TODO 3: inode->i_cdev contains our cdev struct, use container_of to obtain a pointer to so2_device_data */
	data = container_of(inode->i_cdev, struct so2_device_data, cdev);
	file->private_data = data;

	/* TODO 3: return immediately if access is != 0, use atomic_cmpxchg */
	int cpu = smp_processor_id();
	int access = atomic_cmpxchg(&data->dump_lock, -1, cpu);
	if(access != 0)
	{
		return 0;
	}
	
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(10 * HZ);
	
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(1000);
	
	
	return 0;
}

static int so2_cdev_release(struct inode *inode, struct file *file)
{
	/* TODO 2: print message when the device file is closed. */
	pr_info("Device file is closed\n");

#ifndef EXTRA
	struct so2_device_data *data =
		(struct so2_device_data *) file->private_data;

	/* TODO 3: reset access variable to 0, use atomic_set */
	atomic_set(&data->dump_lock, 0);
#endif
	return 0;
}

static ssize_t so2_cdev_read(struct file *file,char __user *user_buffer, size_t size, loff_t *offset)
{
	struct so2_device_data *data = (struct so2_device_data *) file->private_data;
	size_t to_read;

#ifdef EXTRA
	/* TODO 7: extra tasks for home */
#endif
	pr_info("Here!\n");
	/* TODO 4: Copy data->buffer to user_buffer, use copy_to_user */
	size_t i;
	to_read = copy_to_user(user_buffer, data->read_buffer, size);
    if(to_read < 0)
	{
		pr_info("Error reading\n");
		return -1;
	}
	else if(to_read == 0)
	{   
	    pr_info("Before finished reading: Offset: %lld \n", *offset);
		for(i = *offset ; i < size ; i++)
		{
				pr_info("%c", user_buffer[i]);
		}
		*offset = size;
		pr_info("Finised reading: Offset: %lld \n", *offset);
		return 0;
	}
	
	for(i = *offset ; i < to_read ; i++)
	{
		pr_info("%c", user_buffer[i]);
	}
	
	*offset += to_read;
	pr_info("Reading: Offset: %lld \n", *offset);
	return to_read;
}

static ssize_t
so2_cdev_write(struct file *file,
		const char __user *user_buffer,
		size_t size, loff_t *offset)
{
	struct so2_device_data *data =
		(struct so2_device_data *) file->private_data;


	/* TODO 5: copy user_buffer to data->buffer, use copy_from_user */
	/* TODO 7: extra tasks for home */

	return size;
}

static long
so2_cdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct so2_device_data *data =
		(struct so2_device_data *) file->private_data;
	int ret = 0;
	int remains;

	switch (cmd) {
	/* TODO 6: if cmd = MY_IOCTL_PRINT, display IOCTL_MESSAGE */
	/* TODO 7: extra tasks, for home */
	default:
		ret = -EINVAL;
	}

	return ret;
}

static const struct file_operations so2_fops = {
	.owner = THIS_MODULE,
/* TODO 2: add open and release functions */
	.open = so2_cdev_open,
	.release = so2_cdev_release,
/* TODO 4: add read function */
    .read =  so2_cdev_read,
/* TODO 5: add write function */
/* TODO 6: add ioctl function */
};

static int so2_cdev_init(void)
{
	int err;
	int i;

	/* TODO 1: register char device region for MY_MAJOR and NUM_MINORS starting at MY_MINOR */
	err = register_chrdev_region(MKDEV(MY_MAJOR, MY_MINOR), NUM_MINORS,
                             MODULE_NAME);
	if (err != 0) {
		/* report error */
		return err;
	}
	pr_info("Registere the device\n");
	for (i = 0; i < NUM_MINORS; i++) {
#ifdef EXTRA
		/* TODO 7: extra tasks, for home */
#else
		/*TODO 4: initialize buffer with MESSAGE string */
		memcpy(devs[i].read_buffer, MESSAGE, sizeof(MESSAGE));
#endif
		/* TODO 7: extra tasks for home */
		/* TODO 3: set access variable to 0, use atomic_set */
		atomic_set(&devs[i].dump_lock, 0);
		/* TODO 2: init and add cdev to kernel core */
		for(i = 0; i < NUM_MINORS; i++) {
			/*initialize devs[i] fields */
			cdev_init(&devs[i].cdev, &so2_fops);
			cdev_add(&devs[i].cdev, MKDEV(MY_MAJOR, i), 1);
		}
	}

	return 0;
}

static void so2_cdev_exit(void)
{
	int i;

	for (i = 0; i < NUM_MINORS; i++) {
		/* TODO 2: delete cdev from kernel core */
		cdev_del(&devs[i].cdev);
	}

	/* TODO 1: unregister char device region, for MY_MAJOR and NUM_MINORS starting at MY_MINOR */
	unregister_chrdev_region(MKDEV(MY_MAJOR,0),NUM_MINORS);
	pr_info("Unregistered the device\n");
}

module_init(so2_cdev_init);
module_exit(so2_cdev_exit);
