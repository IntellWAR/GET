#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

const int MAX_SIZE = 4096;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    perror("port");
    return 1;
  }
  char buffer[MAX_SIZE];
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in socket_info;
  socket_info.sin_family = AF_INET;
  socket_info.sin_port = htons(atoi(argv[1]));
  socket_info.sin_addr.s_addr = htonl(INADDR_ANY);
  connect(socket_fd, (struct sockaddr *)&socket_info, sizeof(socket_info));

  while (scanf("%s", buffer) == 1) {
    dprintf(socket_fd, "%s ", buffer);
  }
}