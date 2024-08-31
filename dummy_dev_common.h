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
#define NO_OF_DEVICES		(3)		/*dummy_device_1 READONLY, dummy_device_2 WRONLY, dummy_device_3 RWR*/
#define SUCCESS			(0)

typedef enum {READONLY = 0x01, WRONLY, RWR} dummy_dev_permission; /*enum is type safe, more clear and reuseable than macros*/


/*Writing these all variables into our own structure will make the code more reuseable and structured*/
struct dummy_device
{
	struct cdev dum_cdev;			/*struct to hold device*/
	struct file_operations dum_fops;	/*struct to hold device operations*/	
	char *serial_number;			/*holds a unique number for each device*/
	int dum_perm;				/*access permission*/
	char *dum_dev_buff;			/*dummy device storage*/
};
struct dummy_driver 
{
	int total_devices;
	dev_t dum_dev_numb;			/*device number*/
	struct class *dum_dev_class;		/*device class it can be located at /dev/class/device*/	
	struct device *dum_device;		/*device struct */
	struct dummy_device *dev_instance;
};

extern struct dummy_driver *drv_instance;

int dummy_open(struct inode *, struct file *);
loff_t dummy_llseek(struct file *, loff_t, int);
ssize_t dummy_read(struct file *, char __user *, size_t, loff_t *);
ssize_t dummy_write (struct file *, const char __user *, size_t, loff_t *);
long dummy_ioctl(struct file *, unsigned int, unsigned long);
int dummy_release(struct inode *, struct file *);
struct dummy_driver* create_dummy_driver(void);
struct dummy_device* create_dummy_device(void);
int access_perm(int f_mode, int access_mode); 

#endif
