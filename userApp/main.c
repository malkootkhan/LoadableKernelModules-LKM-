#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>


int main()
{

   int count = 10; /*10 bye should be read*/
   char buff[11];

   int fd = open("/dev/dummy_device_1", O_RDONLY);
   if(fd < 0){
      perror("open");
      goto failed;
   }
   printf("File open successful!\n");

   int ret = read(fd, buff, 10);
   if(ret < 0){
      perror("read");
      goto failed;
   }

   printf("readData: %s\n", buff);
   
failed:
   close(fd);

    return 0;
}
