#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/gfp_types.h>
#include <linux/platform_device.h>

/*methods prototypes*/
int platform_drv_open(struct inode *inode, struct file *filp);
ssize_t platform_drv_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
ssize_t platform_drv_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos);
loff_t platform_drv_llseek(struct file *filp, loff_t, int);
int platform_drv_release(struct inode *inode, struct file *filp);
long platform_drv_ioctl(struct file *filp, unsigned int, unsigned long);


/*probe and remove function prototypes*/
int dummy_platform_drv_probe(struct platform_device *);
int dummy_platform_drv_remove(struct platform_device *);

int platform_drv_open(struct inode *inode, struct file *filp)
{

    pr_info("function: %s\n", __func__);

    return 0;
}

ssize_t platform_drv_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
    pr_info("function: %s\n", __func__);
	return 0;
}

ssize_t platform_drv_write (struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
    
    pr_info("function: %s\n", __func__);
	return 0;
}

loff_t platform_drv_llseek(struct file *filp, loff_t offset, int whence)
{
    pr_info("function: %s\n", __func__);
	return 0;
}
long platform_drv_ioctl(struct file *filp, unsigned int, unsigned long)
{
    pr_info("function: %s\n", __func__);
	return 0;
}
int platform_drv_release(struct inode *, struct file *)
{
    pr_info("function: %s\n", __func__);
	return 0;
}



struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = platform_drv_open,
  .read = platform_drv_read,
  .write = platform_drv_write,
  .llseek = platform_drv_llseek,
  .release = platform_drv_release,
  .unlocked_ioctl = platform_drv_ioctl
};

/*Basic implementation of probe and remove function*/
int dummy_platform_drv_probe(struct platform_device *)
{
  pr_info("Device is detected. function: %s\n", __func__);
  return 0;
}

int dummy_platform_drv_remove(struct platform_device *)
{
  pr_info("Device is removed. function: %s\n", __func__);
  return 0;
}

/*1. new thing to be done is creating driver structure instance as we have done for platform device struct platform_device instance*/
struct platform_driver dummy_platform_drv = {
  .probe = dummy_platform_drv_probe,  /*Callback function called on match between device name and driver provided name*/
  .remove = dummy_platform_drv_remove, /*when disconnecting driver from kernel, this callback function is called to ensure removal*/
  .driver = {
    .name = "my_dummy_platform_device" /*should be exactly the same as the device name, so that at match the probe callback function will be called*/
    }
};



static int __init dummy_platform_drv_init(void)
{
      
      platform_driver_register(&dummy_platform_drv);
      printk("dummy platform driver is inserted to kernel successfully\n");
    return 0;
}

static void __exit dummy_platform_drv_exit(void)
{
        platform_driver_unregister(&dummy_platform_drv);
 	printk("dummy platform driver removed from kernel successfully\n");
}


module_init(dummy_platform_drv_init);
module_exit(dummy_platform_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Malkoot Khan");
MODULE_DESCRIPTION("dummy platform driver");
