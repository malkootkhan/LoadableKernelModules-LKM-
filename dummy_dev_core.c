#include "dummy_dev_common.h"

struct dummy_driver* create_dummy_driver(void)
{
	struct dummy_driver *drv;
	drv = kmalloc(sizeof(struct dummy_driver), GFP_KERNEL);
	drv->dev_instance = create_dummy_device();
	return drv;
}

static int __init dummy_driver_init(void){

	int ret;
	int i;
	drv_instance = create_dummy_driver();

	/*alloc device number alloc_chrdev_region()*/
	ret = alloc_chrdev_region(&drv_instance->dum_dev_numb, 0, NO_OF_DEVICES, "dummy-device");
	if(ret < 0){
		printk("Device registration is not successful\n");
		goto alloc_chrdev_error;
	}

	drv_instance->dum_dev_class = class_create("dummy_dum_dev_class");
	if(IS_ERR(drv_instance->dum_dev_class))
	{
		printk("device class couldn't create\n");
		ret = PTR_ERR(drv_instance->dum_dev_class);			/*error code is converted to pointer and return as type is pointer*/
		goto class_create_error;					/*macros like IS_ERR, ERR_PTR, and PTR_ERR used to convert error to pointer and pointer to error code*/

	}

	for(i =0; i < NO_OF_DEVICES; i++)
	{
		char device_unique_name[32];

		sprintf(device_unique_name, "dummy_device_%d", i + 1);

		/*2. init device cdev_init()*/
		cdev_init(&drv_instance->dev_instance[i].dum_cdev, &drv_instance->dev_instance[i].dum_fops);
		drv_instance->dev_instance[i].dum_cdev.owner = THIS_MODULE;

		/*3. add device to the system: cdev_add()*/
	//	dev_t curr_dev_number =  MKDEV(MAJOR(drv_instance->dum_dev_numb), MINOR(drv_instance->dum_dev_numb));
		ret = cdev_add(&drv_instance->dev_instance[i].dum_cdev, drv_instance->dum_dev_numb, 1);
		if(ret < 0){
			printk("device_%d couldn't add to system\n", i+1);
			goto cdev_add_error;
		}

		drv_instance->dum_device = device_create(drv_instance->dum_dev_class, NULL, drv_instance->dum_dev_numb + i, NULL, device_unique_name);

		if(IS_ERR(drv_instance->dum_device)){
			printk("device_%d creation failed\n",i+1);
			ret = PTR_ERR(drv_instance->dum_device);			/*converts pointer to its relevant error code which long int*/
			goto device_create_error;
		}
	}

	printk("devices are created and linked with kernel system successfully\n");
	return SUCCESS;



device_create_error:
		device_destroy(drv_instance->dum_dev_class, drv_instance->dum_dev_numb);
cdev_add_error:
		for (; i >= 0; i--)
			cdev_del(&drv_instance->dev_instance[i].dum_cdev);				/*Follow the sequence of reversing the operations, in case of failure*/
class_create_error:
		class_destroy(drv_instance->dum_dev_class);
alloc_chrdev_error:
		unregister_chrdev_region(drv_instance->dum_dev_numb, NO_OF_DEVICES);				/*basically in if we are handling errors*/
		kfree(drv_instance->dev_instance->dum_dev_buff);
		kfree(drv_instance->dev_instance->serial_number);
		kfree(drv_instance->dev_instance);
		kfree(drv_instance);
		return ret;

}

static void __exit dummy_driver_exit(void){
	
	device_destroy(drv_instance->dum_dev_class, drv_instance->dum_dev_numb);
	class_destroy(drv_instance->dum_dev_class);
	for(int i = 0; i < NO_OF_DEVICES; i++)
	{
		cdev_del(&drv_instance->dev_instance[i].dum_cdev);
		kfree(drv_instance->dev_instance[i].dum_dev_buff);
		kfree(drv_instance->dev_instance[i].serial_number);
	}
	unregister_chrdev_region(drv_instance->dum_dev_numb/*firstDevNo*/, NO_OF_DEVICES/*device cout*/);
	kfree(drv_instance);
	printk("The device is successfully unloaded!\n");

}


module_init(dummy_driver_init);
module_exit(dummy_driver_exit);

