#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h> /*this struct holds the device register/unregister operations and struct platform_device*/

#include "platform.h" /*creating our own header and define our own struct for platform device data for two devices*/

/*3. adding release function, the function pointer is in struct device*/
void	dummy_platform_dev_release (struct device *dev)
{
	/*here the dynamically allcoated memory will be freed if needed*/
	/*when device unregister function takes address of struct platform_device instance that calls release as callback function to free dynamically allocated mmemory*/
  pr_info("Dummy platform device release\n");
}

/*2. create instances for the platform data*/

struct platform_dummy_dev_data platform_dev_data[NO_OF_DEVICES] = {
  [0] = {.size = BUFF_SIZE, .dev_serial_no = "dummy_xyz123", .permission = RDWR  },
  [1] = {.size = BUFF_SIZE, .dev_serial_no = "dummy_xyz456", .permission = RDWR  }
};

/*1. The first and most important thing we do here: create platform device using struct paltform_device*/

/*let create two devices*/
/*there are more members but for now we initize only these two, id shows an index somehow for platform devic*/
struct platform_device dummy_platform_dev_1 = {
  .name = "my_dummy_platform_device",
  .id = 0,
  .dev = {
    .platform_data = &platform_dev_data[0],
    .release =dummy_platform_dev_release
      }
};
struct platform_device dummy_platform_dev_2 = {
  .name = "my_device_platform_device",
  .id = 1,
  .dev = {
    .platform_data = &platform_dev_data[1],
    .release = dummy_platform_dev_release
      }
};
/*as usual we have to register these devices*/
static int __init dummy_platform_dev_init(void)
{
  /*Register platform devices => later should be unregistered in exit function*/
  platform_device_register(&dummy_platform_dev_1);
  platform_device_register(&dummy_platform_dev_2);
  pr_info("Device inserted \n");


  return 0;
}

static void __exit dummy_platform_dev_exit(void)
{
  platform_device_unregister(&dummy_platform_dev_1);
  platform_device_unregister(&dummy_platform_dev_2);
  pr_info("Device removed \n");
}


module_init(dummy_platform_dev_init);
module_exit(dummy_platform_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Malkoot Khan");
MODULE_DESCRIPTION("dummy platform device");
