#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

const int MAX_SIZE = 4096;
const int MAX_CLIENT_COUNT = 128;

struct thread_argument {
  int client_fd;
};

void *work_with_client_tcp(void *arg) {
  char buffer[MAX_SIZE];
  int client_fd = ((struct thread_argument *) arg)->client_fd;
  free(arg);
  int count = 0;
  while ((count = read(client_fd, buffer, sizeof(buffer))) > 0) {
    buffer[count - 1] = '\n';
    write(1, buffer, count);
  }
  close(client_fd);
  pthread_exit(NULL);
}

void accept_clients_tcp(int socket_fd) {
  listen(socket_fd, MAX_CLIENT_COUNT);
  pthread_t thread;
  while (1) {
    int client_fd = accept(socket_fd, (struct sockaddr *) NULL, NULL);
    struct thread_argument *arg = malloc(sizeof(struct thread_argument));
    arg->client_fd = client_fd;
    pthread_create(&thread, NULL, &work_with_client_tcp, arg);
  }
}

void accept_clients_udp(int socket_fd) {
  char buffer[MAX_SIZE];
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(client_addr);
  while (1) {
    int n = recvfrom(socket_fd, buffer, MAX_SIZE, MSG_WAITALL, (struct sockaddr *) &client_addr, &len);
    buffer[n] = '\n';
    buffer[n + 1] = '\0';
    write(1, buffer, n + 1);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    perror("proto, port");
    return 1;
  }
  int socket_fd;
  if (!strncmp(argv[1], "TCP", 3)) {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  } else {
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  }
  struct sockaddr_in socket_info;
  socket_info.sin_family = AF_INET;
  socket_info.sin_port = htons(atoi(argv[2]));
  socket_info.sin_addr.s_addr = INADDR_ANY;
  bind(socket_fd, (struct sockaddr *) &socket_info, sizeof(socket_info));
  if (!strncmp(argv[1], "TCP", 3)) {
    accept_clients_tcp(socket_fd);
  } else {
    accept_clients_udp(socket_fd);
  }
}
