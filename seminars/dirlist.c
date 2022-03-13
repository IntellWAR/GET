#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
  DIR* dir;
  struct dirent* entry;
  if (argc != 2) {
    printf("Invalid number of argumets");
  }
  if (!(dir = opendir(argv[1]))) {
    perror("opendir");
  }
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == 4) {
      printf("%s", entry->d_name);
    }
  }
  closedir(dir);
  return 0;
}