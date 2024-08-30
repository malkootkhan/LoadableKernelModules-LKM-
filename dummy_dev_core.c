#include "dummy_dev_common.h"

static int __init dummy_driver_init(void){

	int ret;
	dev_instance = create_dummy_device();

	/*1. alloc device number alloc_chrdev_region()*/
	ret = alloc_chrdev_region(&dev_instance->dum_dev_numb, 0, 1, "dummy_device_name");
	if(ret < 0){
		printk("Device registration is not successful\n");
		goto alloc_chrdev_error;
	}

	/*2. init device cdev_init()*/
	cdev_init(&dev_instance->dum_cdev, &dev_instance->dum_fops);
	dev_instance->dum_cdev.owner = THIS_MODULE;

	/*3. add device to the system: cdev_add()*/
	ret = cdev_add(&dev_instance->dum_cdev, dev_instance->dum_dev_numb, 1);
	if(ret < 0){
		printk("device couldn't add to system\n");
		goto cdev_add_error;
	}

	dev_instance->dum_dev_class = class_create("dummy_class");
	if(IS_ERR(dev_instance->dum_dev_class))
	{
		printk("device class couldn't create\n");
		ret = PTR_ERR(dev_instance->dum_dev_class);			/*error code is converted to pointer and return as type is pointer*/
		goto class_create_error;					/*macros like IS_ERR, ERR_PTR, and PTR_ERR used to convert error to pointer and pointer to error code*/

	}

	dev_instance->dum_device = device_create(dev_instance->dum_dev_class, NULL, dev_instance->dum_dev_numb, NULL, "dummy_device");
	if(IS_ERR(dev_instance->dum_device)){
		printk("device creation failed\n");
		ret = PTR_ERR(dev_instance->dum_device);			/*converts pointer to its relevant error code which long int*/
		goto device_create_error;
	}
	
	printk("device is successfully integrated into the kernel\n");
	return SUCCESS;


device_create_error:
		class_destroy(dev_instance->dum_dev_class);
class_create_error:
		cdev_del(&dev_instance->dum_cdev);				/*Follow the sequence of reversing the operations, in case of failure*/
cdev_add_error:
		unregister_chrdev_region(dev_instance->dum_dev_numb, 1);	/*basically in if we are handling errors*/
alloc_chrdev_error:
		kfree(dev_instance);
		return ret;

}

static void __exit dummy_driver_exit(void){
	
	device_destroy(dev_instance->dum_dev_class,dev_instance->dum_dev_numb);
	class_destroy(dev_instance->dum_dev_class);
	cdev_del(&dev_instance->dum_cdev);
	unregister_chrdev_region(dev_instance->dum_dev_numb/*firstDevNo*/, 0/*device cout*/);
	kfree(dev_instance);
	printk("The device is successfully unloaded!\n");

}


module_init(dummy_driver_init);
module_exit(dummy_driver_exit);

