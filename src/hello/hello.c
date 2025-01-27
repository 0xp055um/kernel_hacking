#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("A simple Hello World Module");

int __init hello_init(void)
{
	printk(KERN_INFO "Hello World\n");
	return 0;
}

void __exit hello_exit(void)
{
	printk(KERN_INFO "Goodbye World\n");
}

module_init(hello_init);
module_exit(hello_exit);
