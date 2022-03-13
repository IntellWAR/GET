#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define degree 10

int main() {
  pid_t pid;
  int pidstatus = 0;
  FILE *fp;
  fp = fopen("uppid", "w");
  if (fp == NULL) {
    perror("\nOpen");
    return 1;
  }
  for (int i = 0; i < degree; i++) {
    pid = fork();
    if (pid < 0) {
      perror("fork");
      return 2;
    }
    if (pid > 0) {
      waitpid(pid, &pidstatus, 0);
      if (WIFEXITED(pidstatus) != 0) {
        fprintf(fp, "%d:%d\n", pid, WEXITSTATUS(pidstatus));
      } else {
        fprintf(stderr, "ExitErr%d\n", i);
        return 3;
      }
    }
  }

  pid = getpid();
  fprintf(fp, "pid%d\n", pid);
  return 0;
}
