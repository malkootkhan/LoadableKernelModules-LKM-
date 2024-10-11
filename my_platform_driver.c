#include <linux/module.h>
#include <linux/platform_device.h>
#include "platform_dev.h"

/*writing the device driver for the device we already setup*/
int pdrv_probe(struct platform_device *dev)
{

    pr_info("device detected\n");
    return 0;
}
int pdrv_remove(struct platform_device *dev)
{
    pr_info("device removed\n");
    return 0;
}

struct platform_driver pdrv = {
    .probe = pdrv_probe,
    .remove = pdrv_remove,
    .driver = {
        .name = "my_first_pdev"        
    }
};



static int __init init_driver(void)
{
  
    platform_driver_register(&pdrv);
    pr_info("driver registeration\n");
    return 0;
}
static void __exit exit_driver(void)
{

    platform_driver_unregister(&pdrv);
    pr_info("driver removal\n");
}


module_init(init_driver);
module_exit(exit_driver);

MODULE_DESCRIPTION("my platform device driver");
MODULE_AUTHOR("Malkoot Khan");
MODULE_LICENSE("GPL");
