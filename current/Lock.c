#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main(int argc, char** argv) {
  if (argc != 4) {
    printf("Invalid number of arguments");
  }
  off_t start = strtoll(argv[2], NULL, 10);
  off_t len = strtoll(argv[3], NULL, 10);
  int fd;
  if ((fd = open(argv[1], O_WRONLY)) < 0) {
    perror("Open");
  }

  struct flock lock;
  //memset(&lock, 0, sizeof(struct flock));
  lock.l_type = F_WRLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = start;
  lock.l_len = len;
  if (fcntl(fd, F_SETLKW, &lock) < 0) {
  	perror ("Locking");
  }
  sleep (30);
  close (fd);
  return 0;
}