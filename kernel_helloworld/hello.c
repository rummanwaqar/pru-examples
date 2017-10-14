/*
 * Hello world loadable kernel module (LKM) that can display a message
 * in /var/log/kern.log file when the module is loaded and removed. The module
 * can accept an argument when it is loaded
 *
 * From: http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/
 */

#include <linux/init.h>		// macros used to mark up functions; e.g. __init
#include <linux/module.h>	// core header for loading LKMs
#include <linux/kernel.h>	// contains types, macros and functions for kernel

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rumman Waqar");
MODULE_DESCRIPTION("A simple Linux driver for the BBB");
MODULE_VERSION("0.1");

// note: global variables are shared kernel wide. Static restricts scope to within this module
static char* name = "world";
module_param(name, charp, S_IRUGO);	// param description. charp = char ptr, S_IRUHO read/not changed
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");

/*
 * LKM init function
 * static keyword restricts visibility to within this file.
 * __init means that for a built-in driver, function is only used at init and its
 * memory is freed up after that.
 */
static int __init helloBBB_init(void)
{
	pr_info("BBB: Hello %s from the BBB LKM!\n", name);
	return 0;
}

/*
 * clean up function
 */
static void __exit helloBBB_exit(void)
{
	pr_info("BBB: Goodbye %s from the BBB LKM!\n", name);
}

/*
 * module must have init and exit macros
 */
module_init(helloBBB_init);
module_exit(helloBBB_exit);
