/*
 * Character driver maps to /dev/testchar and comes with a helper C program that can run
 * is user space to communicate with this LKM.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>	// header for kernel driver model
#include <linux/fs.h>		// header for linux file system support
#include <asm/uaccess.h>	// required for the copy to user function

#define DEVICE_NAME	"testchar"	// /dev/testchar
#define CLASS_NAME	"test"		// device class (driver) /sys/class/test/testchar

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rumman Waqar");
MODULE_DESCRIPTION("Simple linux char driver");
MODULE_VERSION("0.1");

static int 		majorNumber;					// stores device number - automatically generated
static char 	message[256]={0};				// memory for string passed from userspace
static short 	size_of_message;				// used to remember size of string stored
static int		numberOpens = 0;				// counts times device is opened
static struct class* testcharClass = NULL;
static struct device* testcharDevice = NULL;

// functions for character driver (linux/fs.h)
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

/*
 * devices are represented as file structure in kernel.
 * file_operations structure from linux/fs.h lists cb functions associated with device
 */
static struct file_operations fops =
{
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release
};

/*
 * initialization function
 */
static int __init testchar_init(void)
{
	pr_info("TestChar: Initializing the TestChar LKM\n");

	// dynamically allocate major number
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber > 0)
	{
		pr_alert("TestChar: failed to register a major number\n");
		return majorNumber;
	}
	pr_info("TestChar: registered correctly with major number %d\n", majorNumber);

	// register device class
	testcharClass = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(testcharClass))	// if error -> clean up
	{
		unregister_chrdev(majorNumber, DEVICE_NAME);
		pr_alert("TestChar: failed to register device class\n");
		return PTR_ERR(testcharClass);
	}
	pr_info("TestChar: device class registered correctly");

	// register device driver
	testcharDevice = device_create(testcharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if(IS_ERR(testcharDevice))
	{
		class_destroy(testcharClass);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		pr_alert("TestChar: failed to create the device\n");
		return PTR_ERR(testcharDevice);
	}
	pr_info("TestChar: device class created correctly\n");
	return 0;
}

/*
 * exit function
 */
static void __exit testchar_exit(void)
{
	device_destroy(testcharClass, MKDEV(majorNumber, 0));	// remove device
	class_unregister(testcharClass);// unregister device class
	class_destroy(testcharClass);	// remove device class
	unregister_chrdev(majorNumber, DEVICE_NAME); // unregister major number
	pr_info("TestChar: Goodbye :)");
}

/*
 * device open function is called each time the device is opened
 */
static int dev_open(struct inode* inodep, struct file* filep)
{
	numberOpens++;
	pr_info("TestChar: Device has been opened %d time(s)\n", numberOpens);
	return 0;
}

/*
 * called whenever the device is being read from user space
 * uses copy_to_user() sends a buffer of string to the user
 */
static ssize_t dev_read(struct file* filep, char* buffer, size_t len, loff_t* offset)
{
	int error_count = 0;
	// copy_to_user(*to, *from, size) and returns 0 on success
	error_count = copy_to_user(buffer, message, size_of_message);

	if(error_count == 0)
	{
		pr_info("TestChar: Sent %d characters to user.\n", size_of_message);
		return (size_of_message = 0);
	}
	else
	{
		pr_info("TestChar: Failed to send %d characters to users\n", error_count);
		return -EFAULT;		// failed - return a bad address message (e.g. -14)
	}
}

/*
 * called when the device is being written to from user space
 * data is copied to message along with its length
 */
static ssize_t dev_write(struct file* filep, const char* buffer, size_t len, loff_t* offset)
{
	sprintf(message, "%s(%zu letters)", buffer, len);
	size_of_message = strlen(message);
	pr_info("TestChar: Received %zu characters from the user\n");
	return len;
}

/*
 * device release function whenever the device is closed
 */
static int dev_release(struct inode* inodep, struct file* filep)
{
	pr_info("TestChar: Device successfully closed\n");
	return 0;
}

module_init(testchar_init);
module_exit(testchar_exit);
