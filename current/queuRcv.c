#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define MAXTEXTSIZE 8192

struct msgbufl {
  long mtype;
  char mtext[MAXTEXTSIZE];
} msgp;

int main() {
  int msqid;
  key_t key;
  msgp.mtype = 1;
  key = 3;
  if ((msqid = msgget(key, 0662)) < 0) {
    perror("Get id");
  }

  if (msgrcv(msqid, &msgp, MAXTEXTSIZE, 0, IPC_NOWAIT) < 0) {
    perror("Receiving");
    return 1;
  }
  printf("%s\n", msgp.mtext);
  return 0;
}