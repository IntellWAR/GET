#include "My_ssh.h"

#define MAX_CLIENTS 100
int broadcast_answer(struct sockaddr_in* client, struct sockaddr_in* server);
int terminal(struct sockaddr_in* client, struct sockaddr_in* server, int is_udp);

int main() {
    struct sockaddr_in client;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port   = htons(27312);
    server.sin_addr.s_addr   = htonl(INADDR_ANY);
    broadcast_answer(&client, &server);
    terminal(&client, &server, 1);
}

    
int broadcast_answer(struct sockaddr_in* client, struct sockaddr_in* server) {
    int buf_size = 1000, a = 1;
    char* buf = (char*) calloc(buf_size, sizeof(char));
    int socket_udp = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(socket_udp, SOL_SOCKET, SO_BROADCAST, &a, sizeof(a));
    perror("socket");
    if (bind(socket_udp, (struct sockaddr*) server, sizeof(*server)) < 0) {
        perror("bind");
        exit(1);
    }
    socklen_t client_len = sizeof(*client);
    int len = recvfrom(socket_udp, buf, buf_size, 0, (struct sockaddr*) client, &client_len);
    if (len < 0) {
        perror("recvfrom");
        return -1;
    }
    printf("%s\n", buf);
    printf("Msg from: %d\n", ntohs(client -> sin_port));
    sendto(socket_udp, buf, buf_size, 0, (struct sockaddr*) client, sizeof(*client));
    
    close(socket_udp);
}


int terminal(struct sockaddr_in* client, struct sockaddr_in* server, int is_udp) {
    char *bash_argv[] = {"sh", NULL};
    int fdm, fds;
    int rc, ac = 2, auth = 1;
    char input[150];
    fdm = posix_openpt(O_RDWR);
    if (fdm < 0)
    {
    fprintf(stderr, "Error %d on posix_openpt()\n", errno);
    return 1;
    }

    rc = grantpt(fdm);
    if (rc != 0)
    {
    fprintf(stderr, "Error %d on grantpt()\n", errno);
    return 1;
    }

    rc = unlockpt(fdm);
    if (rc != 0)
    {
    fprintf(stderr, "Error %d on unlockpt()\n", errno);
    return 1;
    }
    int base_socket, fd;
    if (is_udp) {
        base_socket = socket(AF_INET, SOCK_DGRAM, 0);
        bind(base_socket, server, sizeof(*server));
        fd = base_socket;
    } 
    // Open the slave side ot the PTY
    fds = open(ptsname(fdm), O_RDWR);

    // Create the child process
    if (fork()) {
        
        fd_set fd_in;

        // FATHER

        // Close the slave side of the PTY
        close(fds);

        while (1) {
            // Wait for data from standard input and master side of PTY
            FD_ZERO(&fd_in);
            FD_SET(0, &fd_in);
            FD_SET(fdm, &fd_in);

            rc = select(fdm + 1, &fd_in, NULL, NULL, NULL);
        
            if (rc == -1) {
                fprintf(stderr, "Error %d on select()\n", errno);
                exit(1);
            }

            // If data on standard input
            if (FD_ISSET(0, &fd_in)) {
                rc = read(0, input, sizeof(input));
                if (rc > 0) {
                    if (strncmp("send", input, 4)){

                    }
                    // Send data on the master side of PTY
                    write(fdm, input, rc);
                }
                else if (rc < 0) {
                    fprintf(stderr, "Error %d on read standard input\n", errno);
                    exit(1);
                }
                
            }

            // If data on master side of PTY
            if (FD_ISSET(fdm, &fd_in)) {
                rc = read(fdm, input, sizeof(input));
                if (rc > 0) {
                    // Send data on standard output
                    write(1, input, rc);
                }
                else if (rc < 0) {
                    fprintf(stderr, "Error %d on read master PTY\n", errno);
                    exit(1);
                }
            }
        } // End while
    }
    else {
        struct termios slave_orig_term_settings; // Saved terminal settings
        struct termios new_term_settings; // Current terminal settings

        // CHILD

        // Close the master side of the PTY
        close(fdm);

        // Save the defaults parameters of the slave side of the PTY
        rc = tcgetattr(fds, &slave_orig_term_settings);

        // Set RAW mode on slave side of PTY
        new_term_settings = slave_orig_term_settings;
        cfmakeraw (&new_term_settings);
        tcsetattr (fds, TCSANOW, &new_term_settings);

        // The slave side of the PTY becomes the standard input and outputs of the child process
        close(0); // Close standard input (current terminal)
        close(1); // Close standard output (current terminal)
        close(2); // Close standard error (current terminal)

        dup(fds); // PTY becomes standard input (0)
        dup(fds); // PTY becomes standard output (1)
        dup(fds); // PTY becomes standard error (2)

        // Now the original file descriptor is useless
        close(fds);

        // Make the current process a new session leader
        setsid();

        // As the child is a session leader, set the controlling terminal to be the slave side of the PTY
        // (Mandatory for programs like the shell to make them manage correctly their outputs)
        ioctl(0, TIOCSCTTY, 1);

        // Execution of the program
        

            // Build the command line
        execvp("sh", bash_argv);


        // if Error...
        return 1;
    }

    return 0;

}

void print_str_socket(int socket, int is_udp, char* str, struct sockaddr_in* client) {
    if (is_udp) {
        sendto(socket, str, strlen(str), 0, client, sizeof(*client));
    }
    else 
        send(socket, str, strlen(str), 0);
}