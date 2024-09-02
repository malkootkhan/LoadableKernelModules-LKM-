#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {SUCCESS, FAILURE}err_code;

int main(int argc, char *argv[])
{
   if(argc != 2){
      perror("Invalid argument e.g:  <exceutableFile> arg1");
      return -FAILURE;
   }

   int count = atoi(argv[1]); /*10 bye should be read*/
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

    return SUCCESS;
}
