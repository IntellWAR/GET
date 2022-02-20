#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

const int MAX_SIZE = 4096;

int main() {
  int socket_fd, a = 1;
  struct sockaddr_in client_addr;
  char buffer[MAX_SIZE];
  socklen_t len = sizeof(client_addr);

  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in socket_info;
  socket_info.sin_family = AF_INET;
  socket_info.sin_port = htons(27312);
  socket_info.sin_addr.s_addr = htonl(INADDR_ANY);

  setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &a, sizeof(a));
  bind(socket_fd, (struct sockaddr *)&socket_info, sizeof(socket_info));

  while (1) {
    int n = recvfrom(socket_fd, buffer, MAX_SIZE, MSG_WAITALL,
                     (struct sockaddr *)&client_addr, &len);
    buffer[n] = '\n';
    buffer[n + 1] = '\0';
    write(1, buffer, n + 1);
    printf("Msg from: %d\n", ntohs(client_addr.sin_port));
    sendto(socket_fd, buffer, strlen(buffer), MSG_CONFIRM,
           (struct sockaddr *)&client_addr, sizeof(client_addr));
  }
  return 0;
}