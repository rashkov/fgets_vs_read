#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

static unsigned long long
elapsed_us(struct timespec *a, struct timespec *b)
{
	unsigned long long a_p = (a->tv_sec * 1000000ULL) + a->tv_nsec / 1000;
	unsigned long long b_p = (b->tv_sec * 1000000ULL) + b->tv_nsec / 1000;
	return b_p - a_p;
}

int process(int const C){
  int fd = open("./test.txt", O_RDONLY);
  int const BUFSIZE = 2 * C;
  int count;
  char * buf = malloc(BUFSIZE);
  char * str;
  char * nl;
  char * last_nl;
  char * cursor;
  int start = 0;
  int i;
  int lines = 0;
  while(1){
    // Keep reading until we encounter a newline in a newly read chunk
    while(start + C <= BUFSIZE &&
         (count = read(fd, buf + start, C)) > 0 &&
          memchr(buf + start, '\n', count) == NULL){
      start += count;
    }
    if(count == 0 || count == -1){
      if(count == -1){
        perror(NULL);
        close(fd);
        return errno;
      }
      close(fd);
      return lines;
    }
    // We may have gotten multiple newlines in a single read()
    // Iterate through them to grab each complete newline-terminated string
    str = buf;
    last_nl = NULL;
    while((nl = memchr(str, '\n', start + count)) != NULL){
      // newline found and nl points to it
      // replace nl with '\0' to terminate our newly found string
      // buf now contains a complete string, which is what we wanted
      *nl = '\0';
      //printf("full line: %s\n", str);
      lines++;
      str = nl + 1; // look for more strings just past this newline
      // save the last known location of a newline, so we can deal with
      // the remainder / line fragment
      last_nl = nl;
    }
    if(last_nl == NULL) continue;
    // * deal with the line fragment *
    // skip any white-space at the front of fragment
    cursor = last_nl + 1;
    while(cursor <= buf + start + count - 1 && isspace(*cursor)) cursor++;
    // copy fragment to the front of the buffer
    i = 0;
    while(cursor <= buf + start + count - 1){
      buf[i] = *cursor;
      i++;
      cursor++;
    }
    // clear anything after the fragment
    cursor = buf + i;
    for(; cursor <= buf + start + count - 1; cursor++) *cursor = '\0';
    // start = first free byte of buf. Use this for the next read()
    start = i;
  }
}
int main(int argc, char **argv){
  struct timespec begin, end;
  int bytes_per_read;
  printf("%s\t%s\n", "nbytes", "elapsed (us)");
  for(int n = 4; n < 28; n++){
    bytes_per_read = 1<<n;
    clock_gettime(CLOCK_MONOTONIC, &begin);
    process(bytes_per_read);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("%d\t%f\n", bytes_per_read, (double) elapsed_us(&begin, &end));
  }
}
