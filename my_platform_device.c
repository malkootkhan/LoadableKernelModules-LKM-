#include <linux/module.h>
#include <linux/platform_device.h>
#include "platform_dev.h"

struct platform_dev_data my_pdev_data[NO_OF_DEVICES] = {
    {512, "XYZ123_serial_number", RDWR}, 
    {512, "XYZ456_serial_number", RDWR}
};

struct platform_device my_pdev[NO_OF_DEVICES] = {
    [0] = {
        .name = "my_first_pdev",
        .id = 0,
        .dev = {
            .release = release_dynamic_alloc,
            .platform_data = &my_pdev_data[0]
        }
    },
    [1] = {
           .name = "my_second_pdev",
           .id = 1,
           .dev = {
               .release = release_dynamic_alloc,
               .platform_data = &my_pdev_data[1]
           }
       }
};


/*this is callback function releases the dynamically allocated memory*/
void release_dynamic_alloc(struct device *dev)
{
    pr_info("releases dynamically allocated memory if any");
}
/*init module to register platform device*/
static int __init init_driver(void)
{

    /*register the platform devices and driver*/
    for(int device = 0; device < NO_OF_DEVICES; device++)
        platform_device_register(&my_pdev[device]);

    return 0;
}

/*exit module to unregister platform device*/
static void __exit exit_driver(void)
{
    /*unregister platform driver and devices*/

    for(int device = 0; device < NO_OF_DEVICES; device++)
        platform_device_unregister(&my_pdev[device]);
}

/*macro to register the these funcion with kernel*/
module_init(init_driver);
module_exit(exit_driver);
/*license information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Malkoot Khan");
MODULE_DESCRIPTION("Basic platform device driver");
