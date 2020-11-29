# fgets() vs read()

This repository is for comparing the performance of fgets() -- which reads one line at a time -- with using read() to work with larger chunks of data and then processing the newlines ourselves.

We can use memchr, strchr, strsep, or strtok to tokenize the data into lines.

## Partial lines
The challenge is how to deal with partially read lines? 

If we read the following, two bytes at a time:
Take, for instance, the following text file:
```
1 2 3
4 5 6
7 8 9
10 11 12
13 14 15
```

If we read this file two bytes at a time, and print the resulting bytes, we'll get:

```
buf: 1                 
buf: 2 
buf: 3
buf: 4 
buf: 5 
buf: 6
buf: 7 
buf: 8 
buf: 9
buf: 10
buf:  1
buf: 1 
buf: 12
buf: 
1
buf: 3 
buf: 14
buf:  1
buf: 5
```

Each of the above lines is actually printing two bytes / two characters.:
- when it prints the first two lines, it's printing a digit and a space: "1 " and "2 "
- on the following line, it prints a digit and a newline (displayed as a blank line): "3\n"
- when it encounters the number 11 from test.txt, it actually splits it across two reads
    - the first read() is a space and a digit: " 1"
    - the second read() is a digit and a space: "1 "
- it also splits the 13
    - the first read is a newline and a digit: "\n1"
    - the second read is a digit and a space: "3 "

### Code
The above output was produced by the following program:
```
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
```
