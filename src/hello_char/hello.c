#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/uaccess.h>

int major = 0;
int minor = 0;
int dev_nr = 1; // The number of devices we want
int msg_len = 32; // The Size of the message buffer in the kernel

module_param(dev_nr, int, S_IRUGO);
module_param(msg_len, int, S_IRUGO);
module_param(major, int, S_IRUGO);
module_param(minor, int, S_IRUGO);

struct my_device_data {
	char *message;
	struct cdev cdev;
};

struct my_device_data *devices;

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("A simple Hello World Character Module");

int hello_open(struct inode *inode, struct file *filp)
{
	struct my_device_data *dev;
	dev = container_of(inode->i_cdev, struct my_device_data, cdev);
	filp->private_data = dev;

	printk(KERN_INFO "Device opened\n");
	return 0;
}

int hello_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Device closed\n");
	return 0;
}

static ssize_t hello_read(struct file *filp, char __user *buff, size_t count,
			  loff_t *f_pos)
{
	struct my_device_data *dev;
	dev = filp->private_data;

	if (copy_to_user(buff, dev->message, msg_len)) {
		printk(KERN_INFO "Failed to \"copy_to_user\"\n");
		return -EFAULT;
	}
	printk(KERN_INFO "Wrote Message to the User: %s of length %d\n",
	       dev->message, msg_len);
	return 0;
}

static ssize_t hello_write(struct file *filp, const char __user *buff,
			   size_t count, loff_t *f_pos)
{
	struct my_device_data *dev = filp->private_data;
	printk(KERN_INFO "Hello from the write callback\n");

	if (count > msg_len) {
		count = msg_len;
	}

	memset(dev->message, 0, msg_len);

	if (copy_from_user(dev->message, buff, count)) {
		printk(KERN_INFO "Failed to \"copy_from_user\"\n");
		return -EFAULT;
	}

	printk(KERN_INFO "Red a Message from the User: %s of length %lu\n",
	       dev->message, count);
	return count;
}

static struct file_operations hello_fops = { .owner = THIS_MODULE,
					     .read = hello_read,
					     .write = hello_write,
					     .open = hello_open,
					     .release = hello_release };

int __init hello_init(void)
{
	int result, i;
	dev_t devno = 0;

	printk(KERN_INFO "Hello World\n");

	// If the caller provides their own Major and Minor numbers
	if (major) {
		devno = MKDEV(major, minor);
		result = register_chrdev_region(devno, dev_nr, "hello_char");
	} else {
		result = alloc_chrdev_region(&devno, 0, dev_nr, "hello_char");
		major = MAJOR(devno);
		minor = MINOR(devno);
	}

	if (result < 0) {
		printk(KERN_INFO "Failed to get Major number: %d\n", major);
		return result;
	}

	printk(KERN_INFO "Got Major number: %d and Minor number: %d\n", major,
	       minor);

	// Allocating the space for the struct based on the number of devices
	devices = kmalloc(sizeof(struct my_device_data) * dev_nr, GFP_KERNEL);
	if (!devices) {
		result = -ENOMEM;
		return result;
	}
	memset(devices, 0, sizeof(struct my_device_data));

	// Initializing the devices
	for (i = 0; i < dev_nr; i++) {
		devno = MKDEV(major, minor + i);
		cdev_init(&devices[i].cdev, &hello_fops);
		if (cdev_add(&devices[i].cdev, devno, 1))
			printk(KERN_INFO "Failed to add driver %d\n", i);
		printk(KERN_INFO
		       "Create device with: 'mknod /dev/hello_char_%d c %d %d'.\n",
		       i, major, minor + i);

		// Allocating space for the Message Buffer that the struct will hold that
		// can store the messages from and to the User
		devices[i].message = kmalloc(msg_len, GFP_KERNEL);
		memset(devices[i].message, 0, msg_len);

		// Initialize the message buffer
		strcpy(devices[i].message, "Hello World!\n");
	}

	return 0;
}

void __exit hello_exit(void)
{
	int i;
	dev_t devno = MKDEV(major, minor);

	printk(KERN_INFO "Goodbye World\n");

	for (i = 0; i < dev_nr; i++) {
		cdev_del(&devices[i].cdev);
		kfree(devices[i].message);
	}
	kfree(devices);
	unregister_chrdev_region(devno, dev_nr);
}

module_init(hello_init);
module_exit(hello_exit);
