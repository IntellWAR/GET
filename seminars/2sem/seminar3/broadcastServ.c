#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

const int MAX_SIZE = 4096;

int main() {
  int socket_fd, socket2_fd, a = 1;
  struct sockaddr_in client_addr;
  char buffer[MAX_SIZE];
  char buffer2[1024] = "Who are you? I didn't call you!";
  socklen_t len = sizeof(client_addr);

  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  socket2_fd = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(27312);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &a, sizeof(a));
  bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  while (1) {
    int n = recvfrom(socket_fd, buffer, MAX_SIZE, MSG_WAITALL,
                     (struct sockaddr *)&client_addr, &len);

    buffer[n] = '\n';
    buffer[n + 1] = '\0';
    write(1, buffer, n + 1);
    printf("Msg from: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    sendto(socket2_fd, buffer2, strlen(buffer2), MSG_CONFIRM, (struct sockaddr *)&client_addr, sizeof(client_addr));
    printf("\n");
  }
  return 0;
}