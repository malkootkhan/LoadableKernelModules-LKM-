
#define NO_OF_DEVICES       (2)

#define RDONLY                (1)
#define WRONLY                (2)
#define RDWR                  (3)
#define BUFF_SIZE             (512)

void dummy_platform_dev_release (struct device *dev);

struct platform_dummy_dev_data 
{
  int size;
  const char *dev_serial_no;
  int permission;
};
