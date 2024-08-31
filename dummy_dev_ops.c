#include "dummy_dev_common.h"

struct dummy_driver *drv_instance;

struct dummy_device* create_dummy_device(void)
{
	struct dummy_device *dev;
	dev = kmalloc(sizeof(struct dummy_device)*NO_OF_DEVICES, GFP_KERNEL); /*GFP_KERNEL is a global kernel flag used for normal alloc where sleeping or blocking is allowed*/
	char serial_no[25];
	struct file_operations fops = {
		.owner = THIS_MODULE,
		.open = dummy_open,
		.read = dummy_read,
		.write = dummy_write,
		.llseek = dummy_llseek,
		.unlocked_ioctl = dummy_ioctl,
		.release = dummy_release
	};
	for (int i = 0; i < NO_OF_DEVICES; i++)
	{
		sprintf(serial_no, "dummy_dev_serial_no_%d", i+1);
		dev[i].dum_dev_buff = kmalloc(sizeof(char)*MAX_BUFF_SIZE, GFP_KERNEL);
		dev[i].serial_number = kmalloc(sizeof(char)*25, GFP_KERNEL);
		strcpy(dev[i].serial_number, serial_no);
		dev[i].dum_fops = fops;
	}
	dev[0].dum_perm = READONLY;
	dev[1].dum_perm = WRONLY;
	dev[2].dum_perm = RWR;

	return dev;
}
int access_perm(int f_mode, int access_mode )
{

	if(f_mode == RWR)
		return SUCCESS;

	if( (f_mode == READONLY) && (access_mode & FMODE_READ) && !(access_mode & FMODE_WRITE))
		return SUCCESS;
	
	if ( (f_mode == WRONLY) && (access_mode & FMODE_WRITE) && !(access_mode & FMODE_READ))
		return SUCCESS;
	
	return -EPERM;
}
int dummy_open(struct inode *inode, struct file *filp)
{
	int ret;
	struct dummy_device *dev_data;

	/*this is standard kernel macro used to obtained the container structuer address from the inside member */
	dev_data = container_of(inode->i_cdev, struct dummy_device, dum_cdev);

	/*this struct is initialized by kernel when file is open from user space app, we use the private_data pointer to hold data*/
	filp->private_data = dev_data;				
	ret = access_perm(dev_data->dum_perm, filp->f_mode);

	(!ret)?printk("Operation was successful\n"):printk("Operation is not permitted\n");

	return ret;
}

ssize_t dummy_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
	struct dummy_device *dev = filp->private_data;

	printk("TestPoint in %s: Executing from function: ", __func__);
	printk("TestPoint in %s: current file position: %lld", __func__, *f_pos);
	
	/*check the buffer size should not overflow*/
	if (*f_pos+count >= MAX_BUFF_SIZE)
		count = MAX_BUFF_SIZE - *f_pos;
	
	/*bad address macro in errno-base.h*/
	if(copy_to_user(buff, dev->dum_dev_buff + *f_pos, count))
		return -EFAULT; 

	/*update the position value*/
	*f_pos += count;


	printk("TestpoinT in %s: the updated file position: %lld\n",__func__, *f_pos);
	return count;
}

ssize_t dummy_write (struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
	struct dummy_device *dev = filp->private_data;
	printk("TestPoint in %s: current position: %lld", __func__, *f_pos);

	/*check the buffer overflow*/
	if ( *f_pos + count >= MAX_BUFF_SIZE )
		count = MAX_BUFF_SIZE - *f_pos;
	
	/*if the return is non-zero it means there is no space and the kernel has special macro for that  which is ENOMEM in errno-base.h*/ 
	if(!count) {
		printk("There is no memory space left on the device\n");
		return -ENOMEM; 
	}
	/*copy from user tokernel*/
	if(copy_from_user(dev->dum_dev_buff + *f_pos, buff, count))
		return -EFAULT; /*bad address macro in errno-base.h*/
	
	/*update the position*/
	*f_pos += count;

	printk("TestPoint in %s: updated positon: %lld\n", __func__, *f_pos);
	return count;
}

loff_t dummy_llseek(struct file *file, loff_t offset, int whence)
{
	/*
	 * whence can have following values defined in types.h for lseek() 
	*#define SEEK_SET       0
	*#define SEEK_CUR       1
	*#define SEEK_END       2
	I think linux kernel maitainer should replace these macros with enum which will be type safe and reuasability and understandability more clarity but it will be difficult to change at this stage as the kernel is used in million of application
	recommended way:
	typedef enum {
    SEEK_SET = 0,
    SEEK_CUR = 1,
    SEEK_END = 2
} SeekWhence;

e.g:
static long dummy_ioctl(struct file *, unsigned int, SeekWhence whence)
	*/


/*offset is gap from any reference point such as start and to current position, */
loff_t temp ;

printk("TestPoint in %s: current file position: %lld\n", __func__, file->f_pos);

switch(whence){
	case SEEK_SET:
		if(offset > MAX_BUFF_SIZE || offset < 0)
			return -EINVAL; /* Invalid argument */
		file->f_pos = offset;
		break;
	case SEEK_CUR:
		temp = file->f_pos + offset;
		if(temp > MAX_BUFF_SIZE || temp < 0)
			return -EINVAL; /*invalid argument*/
		file->f_pos = temp;
		break;
	case SEEK_END:
		temp = file->f_pos + offset;
		if(temp > MAX_BUFF_SIZE || temp < 0)
			return -EINVAL; /*invalid argument*/
		file->f_pos = temp;
		break;
	default:
		return -EINVAL;
	}
	printk("TestPoint in %s: updated file position: %lld\n", __func__, file->f_pos);
	return file->f_pos;
}
long dummy_ioctl(struct file *, unsigned int, unsigned long)
{
	return 0;
}
int dummy_release(struct inode *, struct file *)
{
	printk("The device is closed\n");
	return 0;
}

