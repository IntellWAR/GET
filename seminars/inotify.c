#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

#define MIN_BUF_LEN (sizeof(struct inotify_event) + 3)

int main(int argc, char** argv) {
  int fd;
  int BUF_LEN;
  BUF_LEN = sizeof(struct inotify_event) + 16;
  char buffer[BUF_LEN + PATH_MAX];
  if ((fd = inotify_init()) < 0) {
    perror("Initialize");
  }
  inotify_add_watch(fd, argv[1], IN_CREATE | IN_DELETE);
  perror("Adding new watch");
  printf("%s\n", argv[1]);
  while (1) {
    int i = 1;
    while (read(fd, buffer, i * MIN_BUF_LEN) <= 0) {
      i++;
    }
    struct inotify_event* event = (struct inotify_event*)buffer;
    if (event->len) {
      if (event->mask & IN_CREATE) {
        printf("The file %s was created with WD %d\n", event->name, event->wd);
      }
      if (event->mask & IN_DELETE) {
        printf("The file %s was deleted with WD %d\n", event->name, event->wd);
      }
      i += sizeof(struct inotify_event) + event->len;
    }
  }
  close(fd);
  return 0;
}