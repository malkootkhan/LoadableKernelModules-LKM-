
#ifndef __PLATFORM__H
#define __PLATFORM__H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/gfp_types.h>
#include <linux/platform_device.h>
#include <linux/device.h>

#define NO_OF_DEVICES       (2)

#define RDONLY                (1)
#define WRONLY                (2)
#define RDWR                  (3)
#define BUFF_SIZE             (512)
#define MAX_DEVICES           (2)

void dummy_platform_dev_release (struct device *dev);

struct platform_dummy_dev_data 
{
  int size;
  const char *dev_serial_no;
  int permission;
};

/*creating our own structures to hold device related info and driver info*/
struct platform_dev_private_data 
{
  struct platform_dummy_dev_data dev_private_data;
  char *buffer;       /*dynamically allocated when probe function is called device detected*/
  dev_t dev_numb;     /*actual device number*/
  struct cdev cdev;   /*device structure*/
};

/*creating driver specific structure*/
struct platform_drv_private_data
{
  int total_devices;
  dev_t dev_numb_base;
  struct device *dummy_device;
  struct class *dummy_class;
};
extern struct platform_drv_private_data dummy_drv_instance;
#endif
