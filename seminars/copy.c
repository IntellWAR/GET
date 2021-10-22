#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int Copy(char* from, char* to, int64_t FileSize);

int64_t FileSizeF(int fd);

int64_t AllRead(int fd, char *buf, int64_t FileSize);

int64_t AllWrite(int fd, char *buf, int FileSize);

int main(int argc, char **argv)
{
  int N, fd;
  int64_t FileSize;
  char *From;
  char *To;
  N = argc;
  if (N != 3) {
    printf("\nInvalid number of arguments");
  }
  From = argv[1];
  To = argv[2];

  fd = open(From, O_RDWR);
  if (fd < 0) {
    perror("\nOpen (size)");
    return 1;
  }
  perror("\nOpen (size)");
  FileSize = FileSizeF(fd);
  printf ("File Size = %li\n", FileSize);
  if (FileSize < 0) {
    printf("Wrong size\n");
    return 2;
  }
  close(fd);
  perror("Closing (size)");

  int CopyResult;
  CopyResult = Copy(From, To, FileSize);
  if (CopyResult != 0) {
    if (CopyResult == 1) {
      printf ("Reading error\n");
      return 3;
    }
    if (CopyResult == 2) {
      printf ("Writing error\n");
      return 4;
    }
  }

  return 0;
}

int Copy(char* from, char* to, int64_t FileSize)
{
  int fd;
  char buf[FileSize];

  fd = open(from, O_RDONLY);
  perror("Descriptor (in)");
  if (AllRead (fd, buf, FileSize) < FileSize) {
    printf ("Not all symbols were read!\n");
    return 1;
  };
  close(fd);
  perror("Closing (in)");

  fd = open(to, O_RDWR | O_CREAT | O_TRUNC, 0644);
  perror("Descriptor (out)");
  if (AllWrite (fd, buf, FileSize)<FileSize) {
    printf ("Not all symbols were written!\n");
    return 2;
  }
  close(fd);
  perror("Closing (out)");
  return 0;
}

int64_t FileSizeF(int fd)
{
  struct stat File;
  if ((fstat(fd, &File) != 0) || (!S_ISREG(File.st_mode))) {
    return -2;
  } else {
    return File.st_size;
  }
}

int64_t AllRead(int fd, char *buf, int64_t FileSize)
{
  int AllReadBytes = 0;
  int ReadBytes = 0;
  while (AllReadBytes < FileSize) {
    ReadBytes = read(fd, buf + ReadBytes, FileSize - ReadBytes);
    if (ReadBytes < 0) {
      perror("Reading");
      return 1;
    }
    perror("Reading");
    AllReadBytes += ReadBytes;
  }
  return AllReadBytes;
}

int64_t AllWrite(int fd, char *buf, int FileSize)
{
  int AllWrittenBytes = 0;
  int WrittenBytes = 0;
  while (AllWrittenBytes < FileSize) {
    WrittenBytes = write(fd, buf + WrittenBytes, FileSize - WrittenBytes);
    if (WrittenBytes < 0) {
      perror("Writing");
      return 1;
    }
    perror("Writing");
    AllWrittenBytes += WrittenBytes;
  }
  return AllWrittenBytes;
}