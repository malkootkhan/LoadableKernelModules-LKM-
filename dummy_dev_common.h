#ifndef __DUMMY_DEV_COMMON__H

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
extern struct dummy_device *dev_instance; /*global instance of dummy device */

int dummy_open(struct inode *, struct file *);
loff_t dummy_llseek(struct file *, loff_t, int);
ssize_t dummy_read(struct file *, char __user *, size_t, loff_t *);
ssize_t dummy_write (struct file *, const char __user *, size_t, loff_t *);
long dummy_ioctl(struct file *, unsigned int, unsigned long);
int dummy_release(struct inode *, struct file *);
struct dummy_device* create_dummy_device(void);

#endif
