obj-m := dummy_dev_drv.o 


dummy_dev_drv-objs := dummy_dev_core.o dummy_dev_ops.o

KERN_REL = $(shell uname -r)
MOD_DIR = /lib/modules
KERN_DIR = $(MOD_DIR)/$(KERN_REL)/build

all:
	make -C $(KERN_DIR) M=$(PWD) modules
help:
	make -C $(KERN_DIR) M=$(PWD) help

clean:
	make -C $(KERN_DIR) M=$(PWD) clean
