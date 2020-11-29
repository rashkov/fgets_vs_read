#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv){
  int fd = open("./test.txt", O_RDONLY);
  int count;
  char buf[100];
  int C = 2;

  while(count = read(fd, buf, C)){
    if(count < 0){
      perror(NULL);
      return errno;
    }
    printf("buf: %s\n", buf);
    // use memchr() or strchr() to detect '\n', determining when we've read a complete line
    // (could possibly use strsep() although I didn't have any luck with that)
    // Goal of the program is to make the following line work
    // printf("The full line is: %s\n"...)
    // set things up for the next loop

  }
}
