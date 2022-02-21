#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

const int MAX_SIZE = 4096;

int main(int argc, char *argv[]) {
  int socket_fd, a = 1;
  char buffer[MAX_SIZE];
  
  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(27312);
  server_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

  socklen_t len = sizeof(server_addr);

  setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &a, sizeof(a));
  bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  sendto(socket_fd, argv[1], strlen(argv[1]), MSG_CONFIRM,
         (struct sockaddr *)&server_addr, sizeof(server_addr));

  int n = recvfrom(socket_fd, buffer, MAX_SIZE, MSG_WAITALL,
                   (struct sockaddr *)&server_addr, &len);
  printf ("%d", n);
  buffer[n] = '\n';
  buffer[n + 1] = '\0';
  write(1, buffer, n + 1);
  printf("Ansv from: %d\n", ntohs(server_addr.sin_port));
  sendto(socket_fd, buffer, strlen(buffer), MSG_CONFIRM,
         (struct sockaddr *)&server_addr, sizeof(server_addr));

  return 0;
}