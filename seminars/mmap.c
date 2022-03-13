#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int64_t FileSizeF(int fd);

int main(int argc, char **argv) {
  int fdin, fdout;
  void *src, *dst;
  int64_t size;

  if (argc != 3) {
    printf("\nInvalid number of arguments");
  }

  if ((fdin = open(argv[1], O_RDONLY)) < 0) {
    perror("Open (input)");
  }
  if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0) {
    perror("Open (output)");
  }

  if ((size = FileSizeF(fdin)) < 0) {
    perror("Size");
  }

  if (lseek(fdout, size - 1, SEEK_SET) == -1) {
    perror("Lseek");
  }
  write(fdout, "", 1);

  if ((src = mmap(0, size, PROT_READ, MAP_SHARED, fdin, 0)) == MAP_FAILED) {
    perror("Mmap (input)");
  }
  if ((dst = mmap (0, size, PROT_WRITE, MAP_SHARED, fdout, 0)) == MAP_FAILED)
  {
    perror("Mmap (output)");
  }
  memcpy(dst, src, size);
  close (fdout);
  close (fdin);
  munmap (src, size);
  munmap (dst, size);

  return 0;
}

int64_t FileSizeF(int fd) {
  struct stat File;
  if ((fstat(fd, &File) != 0) || (!S_ISREG(File.st_mode))) {
    return -1;
  } else {
    return File.st_size;
  }
}