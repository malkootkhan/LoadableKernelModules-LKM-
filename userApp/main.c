#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {SUCCESS, FAILURE}err_code;

#define  READ_MODE      0
#define  WRITE_MODE     1
#define RWR_MODE        2


void help()
{
      perror("Invalid argument e.g:  <exceutableFile> arg1");
      printf("arg1: file path\n");

}

int main(int argc, char *argv[])
{

   if(strcmp(argv[1],"help") == 0) {
      help();
      return SUCCESS;
   }

   if(argc != 3){
      help();
      return -FAILURE;
   }
   int byteCount = atoi(argv[2]);
   char buff[100];
   char file_path[24];
   strcpy(file_path, argv[1]);
   int fd = open(file_path, O_RDONLY);
   if(fd < 0)
   {
      perror("open: ");
      goto out;
   }
   printf("open successful\n");
   int ret = read(fd, buff, byteCount);
   if(ret < 0)
   {
      perror("read: ");
      goto out;
   }
   printf("read successful\n");
   printf("read data: %s\n", buff);


    return SUCCESS;
out:
    close(fd);
    return -FAILURE;
}
