#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/uaccess.h>

int major, minor;

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("A simple Hello World Character Module");

int hello_open(struct inode *inode, struct file *filp) {
  printk(KERN_INFO "Device opened\n");
  return 0;
}
int hello_release(struct inode *inode, struct file *filp) {
  printk(KERN_INFO "Device open\n");
  return 0;
}

static ssize_t hello_read(struct file *filp, char __user *buff, size_t count,
                          loff_t *f_pos) {
  char *msg = "Hello World\n";
  int len = strlen(msg);
  if (copy_to_user(buff, msg, len)) {
    return -EFAULT;
  }
  printk(KERN_INFO "Wrote Message to the User: %s of length %d\n", msg, len);
  return 0;
}

static ssize_t hello_write(struct file *filp, const char __user *buff,
                           size_t count, loff_t *f_pos) {
  char msg[10] = {0};
  if (count > 10){
  	count = 10;
  }
  if(copy_from_user(msg, buff, count)){
  	return -EFAULT;
  }
  printk(KERN_INFO "Red a Message from the User: %s of length %lu\n", msg,
  count);
  return 0;
}

static struct file_operations hello_fops = {.owner = THIS_MODULE,
                                            .read = hello_read,
                                            .write = hello_write,
                                            .open = hello_open,
                                            .release = hello_release};

int __init hello_init(void) {
  printk(KERN_INFO "Hello World\n");
  major = register_chrdev(0, "hello_char", &hello_fops);

  if (major < 0) {
    printk(KERN_ALERT "Failed to get Major number\n");
    return major;
  }

  printk(KERN_INFO "Got Major number: %d \n", major);
  printk(KERN_INFO "Create device with: 'mknod /dev/hello_char c %d 0'.\n", major);

  return 0;
}

void __exit hello_exit(void) {
  printk(KERN_INFO "Goodbye World\n");
  unregister_chrdev(major, "Hello_Char");
}

module_init(hello_init);
module_exit(hello_exit);
