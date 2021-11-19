#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

int out_char = 0, counter = 128, pid = 0;

void one(int signo);
void zero(int signo);
void pid_info(int sig_num, siginfo_t* inf, void* p);

int main(int argc, char** argv) {
  int fdout;
  sigset_t set;

  struct sigaction act_one;
  memset(&act_one, 0, sizeof(act_one));
  act_one.sa_flags = SA_RESTART;
  act_one.sa_handler = one;
  sigfillset(&act_one.sa_mask);
  sigaction(SIGUSR1, &act_one, NULL);

  struct sigaction act_zero;
  memset(&act_zero, 0, sizeof(act_zero));
  act_zero.sa_flags = SA_RESTART;
  act_zero.sa_handler = zero;
  sigfillset(&act_zero.sa_mask);
  sigaction(SIGUSR2, &act_zero, NULL);

  struct sigaction actCHLD;
  memset(&actCHLD, 0, sizeof(actCHLD));
  actCHLD.sa_mask = set;
  actCHLD.sa_flags = SA_SIGINFO;
  actCHLD.sa_sigaction = pid_info;
  sigaction(SIGCHLD, &actCHLD, 0);

  sigaddset(&set, SIGUSR1);
  sigaddset(&set, SIGUSR2);
  sigaddset(&set, SIGCHLD);
  sigprocmask(SIG_BLOCK, &set, NULL);
  sigemptyset(&set);

  if (argc != 2) {
    printf("\nInvalid number of arguments");
  }

  if ((fdout = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
    perror("Open (write)");
    exit(EXIT_FAILURE);
  }

  printf("Pid (receive) = %d\n", getpid());
  sigsuspend(&set);
  // sigsuspend(&set);

  do {
    if (counter == 0) {
      write(fdout, &out_char, 1);
      counter = 128;
      out_char = 0;
    }
    printf("1\n");
    sigsuspend(&set);  // Ждём отправителя
    printf("2\n");
  } while (1);
  return 0;
}

void one(int pid) {
  out_char += counter;
  counter /= 2;
  kill(pid, SIGUSR1);
  printf("one\n");
}

void zero(int pid) {
  counter /= 2;
  kill(pid, SIGUSR1);
  printf("zero\n");
}

void pid_info(int sig_num, siginfo_t* inf, void* p) {
  pid = inf->si_pid;
  printf("Signal received from %i\n", inf->si_pid);
  kill(pid, SIGUSR1);
}