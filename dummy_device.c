#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Malkoot Khan");
MODULE_DESCRIPTION("character driver");

#define DEV_COUNT		(1)
#define DEV_NAME		"chr_device"
#define MY_MINOR		(0)
#define devMinor(dev_count)	(dev_count + MY_MINOR)
#define CLASS_NAME		"chr_dev_class"
#define DEVICE_NAME		"dummy_device"

#define MAX_BUFF_SIZE		(512)

enum err_msg { SUCCESS, DEV_ALLOC_ERR, CLASS_CREAT_ERR, DEV_CREAT_ERR };

typedef int status_t; 

static int dummy_open(struct inode *, struct file *);
static loff_t dummy_llseek(struct file *, loff_t, int);
static ssize_t dummy_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t dummy_write (struct file *, const char __user *, size_t, loff_t *);
static long dummy_ioctl(struct file *, unsigned int, unsigned long);
static int dummy_release(struct inode *, struct file *);

/*struct instances and variable required for a typical dummy character driver*/
	dev_t devNo;				/*device number*/
	struct cdev c_dev;			/*struct to hold device*/
	struct class *dev_class;		/*device class it can be located at /dev/class/device*/	
	struct file_operations fops;		/*struct to hold device operations*/	
	struct device *my_device;		/*device struct */
	char device_buff[MAX_BUFF_SIZE];	/*dummy device storage*/


 static loff_t dummy_llseek(struct file *, loff_t, int)
{
	return 0;
}
static ssize_t dummy_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
	printk("TestPoint in %s: Executing from function: ", __func__);
	printk("TestPoint in %s: current file position: %lld", __func__, *f_pos);
	
	/*check the buffer size should not overflow*/
	if (*f_pos+count >= MAX_BUFF_SIZE)
		count = MAX_BUFF_SIZE - *f_pos;
	
	if(count <= 0)
		return count;

	if(copy_to_user(buff, &device_buff[*f_pos], count))
		return -EFAULT; /*bad address macro in errno-base.h*/

	/*update the position value*/
	*f_pos += count;


	printk("TestpoinT in %s: the updated file position: %lld\n",__func__, *f_pos);
	return 0;
}
static ssize_t dummy_write (struct file *, const char __user *, size_t, loff_t *)
{
	printk("The device is written\n");
	return 0;
}
static long dummy_ioctl(struct file *, unsigned int, unsigned long)
{
	return 0;
}
static int dummy_open(struct inode *, struct file *)
{
	printk("The device is opened\n");
	return 0;
}
static int dummy_release(struct inode *, struct file *)
{
	printk("The device is closed\n");
	return 0;
}

struct file_operations fops = {
		.owner = THIS_MODULE,
		.open = dummy_open,
		.read = dummy_read,
		.write = dummy_write,
		.llseek = dummy_llseek,
		.unlocked_ioctl = dummy_ioctl,
		.release = dummy_release
	};

static int __init dummy_driver_init(void){

	status_t status;

	/*1. alloc device number alloc_chrdev_region()*/
	status = 0;
	status = alloc_chrdev_region(&devNo, 0, 1, "dummy_device_name");
	if(status != 0)
	{
		printk("Device registration is not successful\n");
		return DEV_ALLOC_ERR;
	}

	/*2. init device cdev_init()*/
	cdev_init(&c_dev, &fops);
	c_dev.owner = THIS_MODULE;
	/*3. add device to the system: cdev_add()*/
	status = 0;
	status = cdev_add(&c_dev, devNo, 1);
	if(status < 0){
		printk("device couldn't add to system\n");
		return status;
	}

	dev_class = class_create("dummy_class");
	if(IS_ERR(dev_class))
	{
		printk("device class couldn't create");
		return CLASS_CREAT_ERR;
	}

	my_device = device_create(dev_class, NULL, devNo, NULL, "dummy_device");
	if(IS_ERR(my_device)){
		printk("device creation failed\n");
		return DEV_CREAT_ERR;
	}
	
	printk("device is successfully integrated into the kernel\n");

	return SUCCESS;
}

static void __exit dummy_driver_exit(void){
	
	device_destroy(dev_class, devNo);
	class_destroy(dev_class);
	cdev_del(&c_dev);
	unregister_chrdev_region(devNo/*firstDevNo*/, 0/*device cout*/);
	printk("The device is successfully unloaded!\n");

}


module_init(dummy_driver_init);
module_exit(dummy_driver_exit);

