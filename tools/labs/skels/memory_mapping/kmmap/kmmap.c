/*
 * PSO - Memory Mapping Lab(#11)
 *
 * Exercise #1: memory mapping using kmalloc'd kernel areas
 */

#include <linux/version.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <asm/pgtable.h>
#include <linux/sched/mm.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/highmem.h>
#include <linux/rmap.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include "../test/mmap-test.h"

MODULE_DESCRIPTION("simple mmap driver");
MODULE_AUTHOR("PSO");
MODULE_LICENSE("Dual BSD/GPL");

#define MY_MAJOR	42
/* how many pages do we actually kmalloc */
#define NPAGES		16

/* character device basic structure */
static struct cdev mmap_cdev;

/* pointer to kmalloc'd area */
static void *kmalloc_ptr;

/* pointer to the kmalloc'd area, rounded up to a page boundary */
static char *kmalloc_area;

static int my_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int my_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static int my_read(struct file *file, char __user *user_buffer,
		size_t size, loff_t *offset)
{
	if (size > NPAGES * PAGE_SIZE) {
		return -EIO;	
	}

	memcpy(user_buffer, kmalloc_area, size);

	return size;
}

static int my_write(struct file *file, const char __user *user_buffer,
		size_t size, loff_t *offset)
{
	if (size > NPAGES * PAGE_SIZE) {
		return -EIO;
	}
	memcpy(kmalloc_area, user_buffer, size);
	return size;
}

static int my_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret;
	unsigned long length = vma->vm_end - vma->vm_start;

	/* do not map more than we can */
	if (length > NPAGES * PAGE_SIZE)
		return -EIO;
	
	unsigned long addr = vma->vm_start;
	unsigned long physical_address = virt_to_phys(kmalloc_area);
	unsigned long pfn = physical_address >> PAGE_SHIFT;
	ret = remap_pfn_range(vma, addr, pfn, length, vma->vm_page_prot);
	if (ret < 0) {
		pr_err("Could not map the address area\n");
		return -EIO;
	}
	return 0;
}

static const struct file_operations mmap_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.mmap = my_mmap,
	.read = my_read,
	.write = my_write
};

static int my_seq_show(struct seq_file *seq, void *v)
{
	struct mm_struct *mm;
	struct vm_area_struct *vma_iterator;
	unsigned long total = 0;

	mm = get_task_mm(current);
	vma_iterator = mm->mmap;
	
	while (vma_iterator) {
		pr_info("%lx, %lxn", vma_iterator->vm_start, vma_iterator->vm_end);
		total += vma_iterator->vm_end - vma_iterator->vm_start;
		vma_iterator = vma_iterator->vm_next;	
	}
	mmput(mm);
	seq_printf(seq, "%lu", total);
	return 0;
}

static int my_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, my_seq_show, NULL);
}

static const struct proc_ops my_proc_ops = {
	.proc_open    = my_seq_open,
	.proc_read    = seq_read,
	.proc_lseek   = seq_lseek,
	.proc_release = single_release,
};

static int __init my_init(void)
{
	int ret = 0;
	unsigned int i;
	
	if (!proc_create(PROC_ENTRY_NAME, 0, NULL, &my_proc_ops)) {
		pr_err("could not create procfs entry\n");
		goto out;
	}	

	ret = register_chrdev_region(MKDEV(MY_MAJOR, 0), 1, "mymap");
	if (ret < 0) {
		pr_err("could not register region\n");
		goto out_no_chrdev;
	}

	kmalloc_ptr = kmalloc((NPAGES + 2) * PAGE_SIZE, GFP_KERNEL);
	if (!kmalloc_ptr) 
	       return -ENOMEM;

	kmalloc_area = PAGE_ALIGN((unsigned int)kmalloc_ptr);

	for (i = 0; i < NPAGES * PAGE_SIZE; i+= PAGE_SIZE) {
		struct page *page = virt_to_page(((unsigned int)kmalloc_area) + i);
		SetPageReserved(page);
	}

	unsigned char a = 0xaa;
	unsigned char b = 0xbb;
	unsigned char c = 0xcc;
	unsigned char d = 0xdd;

	for (i = 0; i < NPAGES * PAGE_SIZE; i+= PAGE_SIZE) {
		memcpy(kmalloc_area + i, &a, 1);
		memcpy(kmalloc_area + i + 1, &b, 1);
		memcpy(kmalloc_area + i + 2, &c, 1);
		memcpy(kmalloc_area + i + 3, &d, 1);
	}

	/* Init device. */
	cdev_init(&mmap_cdev, &mmap_fops);
	ret = cdev_add(&mmap_cdev, MKDEV(MY_MAJOR, 0), 1);
	if (ret < 0) {
		pr_err("could not add device\n");
		goto out_kfree;
	}

	return 0;

out_kfree:
	kfree(kmalloc_ptr);
out_unreg:
	unregister_chrdev_region(MKDEV(MY_MAJOR, 0), 1);
out_no_chrdev:
	remove_proc_entry(PROC_ENTRY_NAME, NULL);
out:
	return ret;
}

static void __exit my_exit(void)
{
	int i;

	cdev_del(&mmap_cdev);


	for (i = 0; i < NPAGES * PAGE_SIZE; i+=PAGE_SIZE) {
		struct page *pg = virt_to_page(((unsigned long)kmalloc_area) + i);
		ClearPageReserved(pg);
	}
	kfree(kmalloc_ptr);

	unregister_chrdev_region(MKDEV(MY_MAJOR, 0), 1);
	remove_proc_entry(PROC_ENTRY_NAME, NULL);
}

module_init(my_init);
module_exit(my_exit);
