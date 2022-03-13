#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

const int MAX_SIZE = 4096;
const int MAX_CLIENT_COUNT = 128;

struct thread_argument {
  int client_fd;
};

void *work_with_client(void *arg) {
  char buffer[MAX_SIZE];
  int client_fd = ((struct thread_argument *)arg)->client_fd;
  free(arg);
  int count;
  while ((count = read(client_fd, buffer, sizeof(buffer))) > 0) {
    buffer[count - 1] = '\n';
    write(1, buffer, count);
  }
  close(client_fd);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    perror("port");
    return 1;
  }
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in socket_info;
  socket_info.sin_family = AF_INET;
  socket_info.sin_port = htons(atoi(argv[1]));
  socket_info.sin_addr.s_addr = htonl(INADDR_ANY);
  bind(socket_fd, (struct sockaddr *)&socket_info, sizeof(socket_info));
  listen(socket_fd, MAX_CLIENT_COUNT);

  pthread_t thread;
  while (1) {
    int client_fd = accept(socket_fd, (struct sockaddr *)NULL, NULL);
    struct thread_argument *arg = malloc(sizeof(struct thread_argument));
    arg->client_fd = client_fd;
    pthread_create(&thread, NULL, &work_with_client, arg);
  }
}
