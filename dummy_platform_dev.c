#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h> /*this struct holds the device register/unregister operations and struct platform_device*/


/*1. The first and most important thing we do here: create platform device using struct paltform_device*/

/*let create two devices*/
/*there are more members but for now we initize only these two, id shows an index somehow for platform devic*/
struct platform_device dummy_platform_dev_1 = {
  .name = "my_dummy_platform_device",
  .id = 0
};
struct platform_device dummy_platform_dev_2 = {
  .name = "my_device_platform_device",
  .id = 1
};
/*as usual we have to register these devices*/

static int __init dummy_platform_dev_init(void)
{
  /*Register platform devices => later should be unregistered in exit function*/
  platform_device_register(&dummy_platform_dev_1);
  platform_device_register(&dummy_platform_dev_2);


  return 0;
}

static void __exit dummy_platform_dev_exit(void)
{
  platform_device_unregister(&dummy_platform_dev_1);
  platform_device_unregister(&dummy_platform_dev_2);
}


module_init(dummy_platform_dev_init);
module_exit(dummy_platform_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Malkoot Khan");
MODULE_DESCRIPTION("dummy platform device");
