#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int serror(char *symb);

void IntToChar(int Number, int fd);

int BizzBuzz(char *from, char *to, int64_t FileSize);

int64_t FileSizeF(int fd);

int64_t AllRead(int fd, char *buf, int64_t FileSize);

int AllWrite(int fd, char *buf, int FileSize);

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

  FileSize = FileSizeF(fd);

  printf("File Size = %li\n", FileSize);
  if (FileSize < 0) {
    printf("Wrong size\n");
    return 2;
  }
  if (close(fd) != 0) {
    perror("Closing (size)");
    return 3;
  }

  int BizzBuzzResult;
  BizzBuzzResult = BizzBuzz(From, To, FileSize);
  if (BizzBuzzResult != 0) {
    if (BizzBuzzResult == 1) {
      printf("Reading error\n");
      return 4;
    }
    if (BizzBuzzResult == 2) {
      printf("Writing error\n");
      return 5;
    }
    if (BizzBuzzResult == 3) {
      printf("Incorrect input\n");
      return 6;
    }
    if (BizzBuzzResult == 4) {
      printf("Incorrect input\n");
      return 7;
    }
  }
  return 0;
}

int BizzBuzz(char *from, char *to, int64_t FileSize)
{
  int fd, CheckedSymbs = 0;
  char buf[FileSize];
  int Number;
  char *endOFline;
  char endSymb;
  char *Bizz = "Bizz ";
  char *Buzz = "Buzz ";
  char *BizzBuzz = "BizzBuzz ";

  if ((fd = open(from, O_RDONLY)) == -1) {
    perror("Descriptor (in)");
  }
  if (AllRead(fd, buf, FileSize) != 0) {
    return 1;
  };
  if (close(fd) == -1) {
    perror("Closing (in)");
  }
  if ((fd = open(to, O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1) {
    perror("Descriptor (out)");
  }

  Number = strtol(buf + CheckedSymbs, &endOFline, 10);
  endSymb = *endOFline;
  while (endOFline != buf + CheckedSymbs) {
    if (endSymb != ' ' && endSymb != '\t' && endSymb != '\n') {
      return 3;
    }
    if (Number == 0) {
      write(fd, &"0 ", 2);
    } else {
      if (Number % 15 == 0) {
        if (AllWrite(fd, BizzBuzz, 9) != 0) {
          return 2;
        }
      } else {
        if (Number % 5 == 0) {
          if (AllWrite(fd, Buzz, 5) != 0) {
            return 2;
          }
        } else {
          if (Number % 3 == 0) {
            if (AllWrite(fd, Bizz, 5) != 0) {
              return 2;
            }
          } else {
            IntToChar(Number, fd);
            write(fd, &" ", 1);
          }
        }
      }
    }
    CheckedSymbs = endOFline - buf + 1;
    Number = strtol(buf + CheckedSymbs, &endOFline, 10);
    endSymb = *endOFline;
  }
  if (CheckedSymbs < FileSize) {
    printf("Not all symbs Checked\n");
    return 4;
  }
  if (close(fd) == -1) {
    perror("Closing (out)");
  }
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
      perror("Reading (process)");
      return 1;
    }
    AllReadBytes += ReadBytes;
  }
  if (AllReadBytes < FileSize) {
    perror("Reading (end)");
    return 1;
  }
  return 0;
}

int AllWrite(int fd, char *buf, int FileSize)
{
  int AllWrittenBytes = 0;
  int WrittenBytes = 0;
  while (AllWrittenBytes < FileSize) {
    WrittenBytes = write(fd, buf + WrittenBytes, FileSize - WrittenBytes);
    if (WrittenBytes < 0) {
      perror("Writing");
      return 1;
    }
    AllWrittenBytes += WrittenBytes;
  }
  if (AllWrittenBytes < FileSize) {
    perror("Writing (end)");
    return 1;
  }
  return 0;
}

void IntToChar(int Number, int fd)
{
  int digit;
  int Num = Number;
  char NumberC;
  digit = Number % 10;
  NumberC = digit + '0';
  if (!Num) {
    return;
  }
  Num = Num / 10;
  IntToChar(Num, fd);
  if (write(fd, &NumberC, 1) == -1) {
    perror("Writing (IntToChar)");
  }
  return;
}