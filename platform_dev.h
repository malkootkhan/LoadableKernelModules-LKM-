#ifndef __PLATFORM_DEV__H
#define __PLATFORM_DEV__H
#include <linux/platform_device.h>
#include <linux/cdev.h>

#define     NO_OF_DEVICES       (2)
#define     RDWR                (1)
#define     RDONLY              (2)
#define     WRONLY              (3)

void release_dynamic_alloc(struct device *dev);

struct platform_dev_data {
    int size;
    const char *serial_number;
    int permission;
    struct cdev cdev;
};

#endif

