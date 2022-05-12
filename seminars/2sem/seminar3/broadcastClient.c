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
    int socket_fd, a = 1;
    char buffer[MAX_SIZE];

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr, client_addr;

    memset(&server_addr, '0', sizeof(server_addr));
    memset(&client_addr, '0', sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(27312);
    server_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    socklen_t len = sizeof(server_addr);

    setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &a, sizeof(a));
    // bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while (1) {
        printf("Enter message:\n");
        read(0, buffer, MAX_SIZE);
        sendto(socket_fd, buffer, strlen(buffer), MSG_CONFIRM,
               (struct sockaddr *)&server_addr, sizeof(server_addr));
        int n = recvfrom(socket_fd, buffer, MAX_SIZE, MSG_WAITALL,
                         (struct sockaddr *)&client_addr, &len);
        buffer[n] = '\n';
        buffer[n + 1] = '\0';
        write(1, buffer, n);
        printf("\nAnsv from: %s:%d\n\n", inet_ntoa(client_addr.sin_addr),
               ntohs(server_addr.sin_port));
        memset(&client_addr, '0', sizeof(server_addr));
        memset(buffer, '\0', sizeof(buffer));
    }
    return 0;
}
