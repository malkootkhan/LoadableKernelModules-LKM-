#include <linux/module.h>
#include "platform_dev.h"

struct platform_dev_data my_pdev_data[NO_OF_DEVICES] = {
    {512, "XYZ123_serial_number", RDWR}, 
    {512, "XYZ456_serial_number", RDWR}
};




/*init module to register platform device*/
static int __init init_driver(void)
{

    /*register the platform devices and driver*/
    platform_device_register(&my_pdev_1);
    platform_device_register(&my_pdev_2);

    return 0;
}

/*exit module to unregister platform device*/
static void __exit exit_driver(void)
{
    /*unregister platform driver and devices*/
    platform_device_unregister(&my_pdev_1);
    platform_device_unregister(&my_pdev_2);

}

/*macro to register the these funcion with kernel*/
module_init(init_driver);
module_exit(exit_driver);
/*license information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Malkoot Khan");
MODULE_DESCRIPTION("Basic platform device driver");
