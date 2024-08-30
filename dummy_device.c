#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/gfp_types.h>

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
/*Writing these all variables into our own structure will make the code more reuseable and structured*/
struct dummy_device
{
	dev_t dum_dev_numb;			/*device number*/
	struct cdev dum_cdev;			/*struct to hold device*/
	struct class *dum_dev_class;		/*device class it can be located at /dev/class/device*/	
	struct file_operations dum_fops;	/*struct to hold device operations*/	
	struct device *dum_device;		/*device struct */
	char dum_dev_buff[MAX_BUFF_SIZE];	/*dummy device storage*/
};
struct dummy_device *dev_instance; /*global instance of dummy device */

static int dummy_open(struct inode *, struct file *);
static loff_t dummy_llseek(struct file *, loff_t, int);
static ssize_t dummy_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t dummy_write (struct file *, const char __user *, size_t, loff_t *);
static long dummy_ioctl(struct file *, unsigned int, unsigned long);
static int dummy_release(struct inode *, struct file *);
static struct dummy_device* create_dummy_device(void);


static int dummy_open(struct inode *, struct file *)
{
	printk("The device is opened\n");
	return 0;
}

static ssize_t dummy_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
	printk("TestPoint in %s: Executing from function: ", __func__);
	printk("TestPoint in %s: current file position: %lld", __func__, *f_pos);
	
	/*check the buffer size should not overflow*/
	if (*f_pos+count >= MAX_BUFF_SIZE)
		count = MAX_BUFF_SIZE - *f_pos;
	

	if(copy_to_user(buff, &dev_instance->dum_dev_buff[*f_pos], count))
		return -EFAULT; /*bad address macro in errno-base.h*/

	/*update the position value*/
	*f_pos += count;


	printk("TestpoinT in %s: the updated file position: %lld\n",__func__, *f_pos);
	return count;
}

static ssize_t dummy_write (struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
	printk("TestPoint in %s: current position: %lld", __func__, *f_pos);
	/*check the buffer overflow*/
	if ( *f_pos + count >= MAX_BUFF_SIZE )
		count = MAX_BUFF_SIZE - *f_pos;

	if(!count) {
		printk("There is no memory space left on the device\n");
		return -ENOMEM; /*if the return is non-zero it means there is no space and the kernel has special macro for that  which is ENOMEM in errno-base.h*/ 
	}
	/*copy from user tokernel*/
	if(copy_from_user(&dev_instance->dum_dev_buff[*f_pos], buff, count))
		return -EFAULT; /*bad address macro in errno-base.h*/
	
	/*update the position*/
	*f_pos += count;

	printk("TestPoint in %s: updated positon: %lld\n", __func__, *f_pos);
	return count;
}

static loff_t dummy_llseek(struct file *file, loff_t offset, int whence)
{
	/*
	 * whence can have following values defined in types.h for lseek() 
	*#define SEEK_SET       0
	*#define SEEK_CUR       1
	*#define SEEK_END       2
	I think linux kernel maitainer should replace these macros with enum which will be type safe and reuasability and understandability more clarity but it will be difficult to change at this stage as the kernel is used in million of application
	recommended way:
	typedef enum {
    SEEK_SET = 0,
    SEEK_CUR = 1,
    SEEK_END = 2
} SeekWhence;

e.g:
static long dummy_ioctl(struct file *, unsigned int, SeekWhence whence)
	*/


/*offset is gap from any reference point such as start and to current position, */
loff_t temp ;

printk("TestPoint in %s: current file position: %lld\n", __func__, file->f_pos);

switch(whence){
	case SEEK_SET:
		if(offset > MAX_BUFF_SIZE || offset < 0)
			return -EINVAL; /* Invalid argument */
		file->f_pos = offset;
		break;
	case SEEK_CUR:
		temp = file->f_pos + offset;
		if(temp > MAX_BUFF_SIZE || temp < 0)
			return -EINVAL; /*invalid argument*/
		file->f_pos = temp;
		break;
	case SEEK_END:
		temp = file->f_pos + offset;
		if(temp > MAX_BUFF_SIZE || temp < 0)
			return -EINVAL; /*invalid argument*/
		file->f_pos = temp;
		break;
	default:
		return -EINVAL;
	}
	printk("TestPoint in %s: updated file position: %lld\n", __func__, file->f_pos);
	return file->f_pos;
}
static long dummy_ioctl(struct file *, unsigned int, unsigned long)
{
	return 0;
}
static int dummy_release(struct inode *, struct file *)
{
	printk("The device is closed\n");
	return 0;
}
static struct dummy_device* create_dummy_device(void)
{
	struct dummy_device *dev;
	dev = kmalloc(sizeof(struct dummy_device), GFP_KERNEL); /*GFP_KERNEL is a global kernel flag used for normal alloc where sleeping or blocking is allowed*/

	struct file_operations fops = {
		.owner = THIS_MODULE,
		.open = dummy_open,
		.read = dummy_read,
		.write = dummy_write,
		.llseek = dummy_llseek,
		.unlocked_ioctl = dummy_ioctl,
		.release = dummy_release
	};
	dev->dum_fops = fops;
	return dev;
}

static int __init dummy_driver_init(void){

	int ret;
	dev_instance = create_dummy_device();

	/*1. alloc device number alloc_chrdev_region()*/
	ret = alloc_chrdev_region(&dev_instance->dum_dev_numb, 0, 1, "dummy_device_name");
	if(ret < 0)
	{
		printk("Device registration is not successful\n");
		kfree(dev_instance);
		return ret;
	}

	/*2. init device cdev_init()*/
	cdev_init(&dev_instance->dum_cdev, &dev_instance->dum_fops);
	dev_instance->dum_cdev.owner = THIS_MODULE;

	/*3. add device to the system: cdev_add()*/
	ret = cdev_add(&dev_instance->dum_cdev, dev_instance->dum_dev_numb, 1);
	if(ret < 0){
		printk("device couldn't add to system\n");
		unregister_chrdev_region(dev_instance->dum_dev_numb, 1); /*basically in if we are handling errors*/
		kfree(dev_instance); /*it should be deallocated, to prevent memory leak */
		return ret;
	}

	dev_instance->dum_dev_class = class_create("dummy_class");
	if(IS_ERR(dev_instance->dum_dev_class))
	{

		printk("device class couldn't create\n");
		cdev_del(&dev_instance->dum_cdev);				/*Follow the sequence of reversing the operations, in case of failure*/
		unregister_chrdev_region(dev_instance->dum_dev_numb, 1);	/*basically in if we are handling errors*/
		kfree(dev_instance);						/*it should be deallocated, to prevent memory leak */
		return PTR_ERR(dev_instance->dum_dev_class);			/*error code is converted to pointer and return as type is pointer*/
										/*macros like IS_ERR, ERR_PTR, and PTR_ERR used to convert error to pointer and pointer to error code*/
	}

	dev_instance->dum_device = device_create(dev_instance->dum_dev_class, NULL, dev_instance->dum_dev_numb, NULL, "dummy_device");
	if(IS_ERR(dev_instance->dum_device)){
		printk("device creation failed\n");
		class_destroy(dev_instance->dum_dev_class);
		cdev_del(&dev_instance->dum_cdev);
		unregister_chrdev_region(dev_instance->dum_dev_numb, 1);	/*device number and device count*/
		kfree(dev_instance);
		return PTR_ERR(dev_instance->dum_device);			/*converts pointer to its relevant error code which long int*/
	}
	
	printk("device is successfully integrated into the kernel\n");

	return SUCCESS;
}

static void __exit dummy_driver_exit(void){
	
	device_destroy(dev_instance->dum_dev_class,dev_instance->dum_dev_numb);
	class_destroy(dev_instance->dum_dev_class);
	cdev_del(&dev_instance->dum_cdev);
	unregister_chrdev_region(dev_instance->dum_dev_numb/*firstDevNo*/, 0/*device cout*/);
	kfree(dev_instance);
	printk("The device is successfully unloaded!\n");

}


module_init(dummy_driver_init);
module_exit(dummy_driver_exit);

