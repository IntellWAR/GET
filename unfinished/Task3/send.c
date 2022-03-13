#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void empty() {
  printf("snd33\n");
  return;
}

int main(int argc, char **argv) {
  int fdin, pid;
  sigset_t set;

  sigaddset(&set, SIGUSR1);
  struct sigaction act_empty;
  memset(&act_empty, 0, sizeof(act_empty));
  act_empty.sa_flags = SA_RESTART;
  act_empty.sa_handler = empty;
  sigfillset(&act_empty.sa_mask);
  sigaction(SIGUSR1, &act_empty, NULL);
  sigemptyset(&set);

  if (argc != 3) {
    printf("\nInvalid number of arguments");
  }
  pid = strtol(argv[2], 0, 10);
  if (pid == LONG_MAX || pid == LONG_MIN || pid == 0) {
    perror("Pid");
  }

  if ((fdin = open(argv[1], O_RDONLY)) < 0) {
    perror("Open (read)");
    exit(EXIT_FAILURE);
  }

  int i;
  char c;
  kill(pid, SIGCHLD);
  sigsuspend(&set);

  while (read(fdin, &c, 1) > 0) {
    for (i = 128; i >= 1; i /= 2) {
      if (i & c) {  // 1
        kill(pid, SIGUSR1);
        printf("sndOne\n");
      } else {  // 0
        kill(pid, SIGUSR2);
        printf("sndZero\n");
      }
      // Ждём подтверждения от получателя
      printf("snd3\n");
      sigsuspend(&set);
      printf("snd4\n");
    }
  }
  return 0;
}