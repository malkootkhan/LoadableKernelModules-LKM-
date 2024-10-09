#include "platform.h"

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
int dummy_platform_drv_probe(struct platform_device *pdev)
{
  /*our own device data structure*/
  struct dummy_pdev_private_data *pdev_private_data;
  /*temporary pointer to */
  struct dummy_platform_data *pdata;
  /*taking data from pdev->get_private_data*/
  pdata = (struct dummy_platform_data *)platform_get_drvdata(pdev);
  if(!pdata)
  {
    pr_info("no platform data is available\n");
    return -EINVAL;
  }

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

struct platform_drv_private_data dummy_drv_instance;

static int __init dummy_platform_drv_init(void)
{
      int ret;
      /*1. allocate device number dynamically*/
	ret = alloc_chrdev_region(&dummy_drv_instance.dev_numb_base, 0, MAX_DEVICES, "dummy_driver");
	if(ret < 0){
		pr_err("Device allocation failed\n");
		return ret;
	}
      dummy_drv_instance.dummy_class = class_create("platform_drv_class");
      if(IS_ERR(dummy_drv_instance.dummy_class))
      {
        pr_err("Device class creation failed\n");
        ret = PTR_ERR(dummy_drv_instance.dummy_class);
        unregister_chrdev_region(dummy_drv_instance.dev_numb_base, MAX_DEVICES);
        return ret;
      }
      platform_driver_register(&dummy_platform_drv);
      pr_info("dummy platform driver is inserted successfully\n");
    return 0;
}

static void __exit dummy_platform_drv_exit(void)
{

        platform_driver_unregister(&dummy_platform_drv);
        class_destroy(dummy_drv_instance.dummy_class);
        unregister_chrdev_region(dummy_drv_instance.dev_numb_base, MAX_DEVICES);
 	pr_info("dummy platform driver removed successfully\n");
}


module_init(dummy_platform_drv_init);
module_exit(dummy_platform_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Malkoot Khan");
MODULE_DESCRIPTION("dummy platform driver");
