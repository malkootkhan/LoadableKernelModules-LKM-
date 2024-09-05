#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/gfp_types.h>

/*methods prototypes*/
int platform_drv_open(struct inode *inode, struct file *filp);
ssize_t platform_drv_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
ssize_t platform_drv_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos);
loff_t platform_drv_llseek(struct file *filp, loff_t, int);
int platform_drv_release(struct inode *inode, struct file *filp);
long platform_drv_ioctl(struct file *filp, unsigned int, unsigned long);


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

static int __init dummy_platform_drv_init(void)
{

 	printk("dummy platform driver inserted to kernel successfully\n");
  return 0;
}

static void __exit dummy_platform_drv_exit(void)
{

 	printk("dummy platform driver removed from kernel successfully\n");
}


module_init(dummy_platform_drv_init);
module_exit(dummy_platform_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Malkoot Khan");
MODULE_DESCRIPTION("dummy platform driver");
