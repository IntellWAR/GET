#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Invalid number of arguments");
  }
  int fd;
  if ((fd = open(argv[1], O_RDONLY)) < 0) {
    perror("Open");
  }
  struct flock lock;
  memset(&lock, 0, sizeof(struct flock));
  if (fcntl(fd, F_GETLK, &lock) < 0) {
    perror("Checking lock");
  }
  if (lock.l_type == F_UNLCK) {
    printf("F_UNLCK\n");
  } else if (lock.l_type == F_RDLCK) {
    printf("F_RDLCK\n");
  } else if (lock.l_type == F_WRLCK) {
    printf("F_WRLCK\n");
  }
  close(fd);
  return 0;
}