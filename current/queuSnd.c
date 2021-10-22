#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

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
  if ((msqid = msgget(key, IPC_CREAT | 0662)) < 0) {
    perror("Get id");
  }
  printf("Введите сообщение\n");
  scanf("%s", msgp.mtext);
  printf("\n");
  if (msgsnd(msqid, &msgp, MAXTEXTSIZE, IPC_NOWAIT) != 0) {
    perror("Sending");
    return 1;
  }
  return 0;
}