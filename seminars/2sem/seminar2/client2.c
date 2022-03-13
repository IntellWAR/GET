#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int MAX_SIZE = 4096;
const char* addr = "192.168.1.51";

int main(int argc, char *argv[]) {
  if (argc < 3) {
    perror("proto, port");
    return 1;
  }
  char buffer[MAX_SIZE];
  int socket_fd;
  if (!strncmp(argv[1], "TCP", 3)) {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  } else {
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  }
  struct sockaddr_in socket_info;
  socket_info.sin_family = AF_INET;
  socket_info.sin_port = htons(atoi(argv[2]));
  socket_info.sin_addr.s_addr = inet_addr(addr);
  if (!strncmp(argv[1], "TCP", 3)) {
    connect(socket_fd, (struct sockaddr *) &socket_info, sizeof(socket_info));
    while (scanf("%s", buffer) == 1) {
      dprintf(socket_fd, "%s ", buffer);
    }
  } else {
    while (scanf("%s", buffer) == 1) {
      sendto(socket_fd, buffer, strlen(buffer), MSG_CONFIRM, (struct sockaddr *) &socket_info, sizeof(socket_info));
    }
  }

}
