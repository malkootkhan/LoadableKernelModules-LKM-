#include "dummy_dev_common.h"

struct dummy_device *dev_instance;

int dummy_open(struct inode *, struct file *)
{
	printk("The device is opened\n");
	return 0;
}

ssize_t dummy_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
	printk("TestPoint in %s: Executing from function: ", __func__);
	printk("TestPoint in %s: current file position: %lld", __func__, *f_pos);
	
	/*check the buffer size should not overflow*/
	if (*f_pos+count >= MAX_BUFF_SIZE)
		count = MAX_BUFF_SIZE - *f_pos;
	

	if(copy_to_user(buff, &dev_instance->dum_dev_buff[*f_pos], count))
		return -EFAULT; /*bad address macro in errno-base.h*/

	/*update the position value*/
	*f_pos += count;


	printk("TestpoinT in %s: the updated file position: %lld\n",__func__, *f_pos);
	return count;
}

ssize_t dummy_write (struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
	printk("TestPoint in %s: current position: %lld", __func__, *f_pos);
	/*check the buffer overflow*/
	if ( *f_pos + count >= MAX_BUFF_SIZE )
		count = MAX_BUFF_SIZE - *f_pos;

	if(!count) {
		printk("There is no memory space left on the device\n");
		return -ENOMEM; /*if the return is non-zero it means there is no space and the kernel has special macro for that  which is ENOMEM in errno-base.h*/ 
	}
	/*copy from user tokernel*/
	if(copy_from_user(&dev_instance->dum_dev_buff[*f_pos], buff, count))
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
struct dummy_device* create_dummy_device(void)
{
	struct dummy_device *dev;
	dev = kmalloc(sizeof(struct dummy_device), GFP_KERNEL); /*GFP_KERNEL is a global kernel flag used for normal alloc where sleeping or blocking is allowed*/

	struct file_operations fops = {
		.owner = THIS_MODULE,
		.open = dummy_open,
		.read = dummy_read,
		.write = dummy_write,
		.llseek = dummy_llseek,
		.unlocked_ioctl = dummy_ioctl,
		.release = dummy_release
	};
	dev->dum_fops = fops;
	return dev;
}

