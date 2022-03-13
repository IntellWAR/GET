#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void handler1();
void handler2();

int main() {
  struct sigaction act1;
  sigset_t set;
  act1.sa_handler = handler1;
  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  sigaddset(&set, SIGUSR2);
  act1.sa_mask = set;
  act1.sa_flags = SA_RESTART;

  struct sigaction act2;
  act2.sa_flags = SA_RESTART;
  act2.sa_handler = handler2;
  act2.sa_mask = set;

  printf("pid =%i\n", getpid());
  sigaction(SIGUSR1, &act1, 0);
  sigaction(SIGUSR2, &act2, 0);
  while (1) {
    // sleep(1);
  }
  return 0;
}

void handler1() {
  printf("London\n");
  return;
}

void handler2() {
  printf("is the capital of Great Britain\n");
  return;
}