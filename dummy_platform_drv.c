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
MODULE_DESCRIPTION("dummy platform driver");


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
