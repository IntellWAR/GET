#include "My_ssh.h"

int find_server(struct sockaddr_in* server);
int send_command_udp(struct sockaddr_in* server, int socket_udp);
int write_mess_tcp(mess_t message, int fd, char* buf);
int get_file_udp(struct sockaddr_in* server, int socket_udp);
int send_file_udp(struct sockaddr_in* server, int socket_udp);

#define BUF_SIZE 1000
char buf[BUF_SIZE];

int main(){
    struct sockaddr_in server;
    int is_udp = 0;
    find_server(&server);
    char user_name[BUF_SIZE];
    
    printf("To use udp press 1, to use tcp press 0\n");
    mess_t connection_type;
    scanf("%d", &is_udp);
    if (is_udp) {
        int socket_udp = socket(AF_INET, SOCK_DGRAM, 0);
        //sendto(socket_udp, &connection_type, sizeof(connection_type), 0, (struct sockaddr*) &server, sizeof(server));
        while(1) {
            int ret = read(STDIN_FILENO, buf, BUF_SIZE);
            ERROR_EXIT(ret, "read");
            if (strncmp(buf, "send", 4))
                    send_file_udp(&server, socket_udp);
            else if (strncmp(buf, "get", 3))
                get_file_udp(&server, socket_udp);
            else if (strncmp(buf, "exit", 4))
                exit(0);
            else send_command_udp(&server, socket_udp);
        }
    }
}

int find_server(struct sockaddr_in* server_info) {
    char buf_receive[BUF_SIZE];

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(27312);
    server_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    sprintf(buf, "%s", "Msg to server\n");
    
    int socket_udp = socket(AF_INET, SOCK_DGRAM, 0);
    perror("socket\n");
    int a = 1;
    setsockopt(socket_udp, SOL_SOCKET, SO_BROADCAST, &a, sizeof(a));

    sendto(socket_udp, buf, BUF_SIZE, 0, (struct sockaddr*) &server_addr, sizeof(server_addr));

    socklen_t server_len = sizeof(*server_info);
    recvfrom(socket_udp, buf_receive, BUF_SIZE, 0, (struct sockaddr*) server_info, &server_len);
    printf("get message back %s: %s\n", buf_receive, inet_ntoa((((struct sockaddr_in*) server_info)->sin_addr)));
    close(socket_udp);
    return 0;
    }


int write_mess_tcp(mess_t message, int fd, char* buf) {
    int ret = write(fd, &message, sizeof(message));
    if (ret < 0) 
        perror("write");
    ret = write(fd, buf, message.mess_size);
    if (ret < 0) 
        perror("write");
    return ret;
}

int write_mess_udp(int socket_udp, mess_t message, char* buf, struct sockaddr_in* server) {
    int ret = sendto(socket_udp, &message, sizeof(message), 0, (struct sockaddr*) server, sizeof(*server));
    if (ret < 0) 
        perror("write");
    ret = sendto(socket_udp, buf, message.mess_size, 0, (struct sockaddr*) server, sizeof(*server));
    if (ret < 0) 
        perror("write");
    return ret;
}

int send_file_udp(struct sockaddr_in* server, int socket_udp) {
    int ret = read(STDOUT_FILENO, buf, BUF_SIZE);//reading file name
    ERROR_EXIT(ret, "read");

    char path_str[BUF_SIZE] = {0};//reading path where to put file
    ret = read(STDIN_FILENO, path_str, BUF_SIZE);
    ERROR_EXIT(ret, "read");

    int file_fd = open(buf, O_RDONLY);
    if (file_fd < 0) {
        perror("open");
        return -1;
    }
    struct stat statistica;
    int stat_error = stat (buf, &statistica);
    assert(stat_error == 0);

    char* file_buf = (char*) calloc(statistica.st_size, sizeof(char));
    ret = read(file_fd, file_buf, statistica.st_size);
    ERROR_EXIT(ret, "read");

    mess_t file = {FILE_BUF, statistica.st_size};
    write_mess_udp(socket_udp, file, file_buf, server);

    mess_t path = {FILE_PATH, strlen(path_str)};
    write_mess_udp(socket_udp, path, path_str, server);

    socklen_t server_len = sizeof(server);
    recvfrom(socket_udp, buf, BUF_SIZE, 0, (struct sockaddr*) &server, &server_len);
    if (((mess_t*) buf) -> mess_type != FILE_SENDING_SUCCESS) {
            printf("problem with file sending");
            exit(1);
        }
}

int get_file_udp(struct sockaddr_in* server, int socket_udp) {
    int ret = read(STDOUT_FILENO, buf, BUF_SIZE);//reading file name
    ERROR_EXIT(ret, "read");

    char path_str[BUF_SIZE] = {0};//reading path where to put file
    ret = read(STDIN_FILENO, path_str, BUF_SIZE);
    ERROR_EXIT(ret, "read");

    mess_t file = {FILE_REQUEST, strlen(buf)};
    write_mess_udp(socket_udp, file, buf, server);
    socklen_t server_len = sizeof(server);
    recvfrom(socket_udp, buf, BUF_SIZE, 0, (struct sockaddr*) server, &server_len);
    if (((mess_t*) buf) -> mess_type != FILE_SENDING_SUCCESS) {
            printf("problem with file sending\n");
            exit(1);
        }

    recvfrom(socket_udp, (char*) &file, sizeof(file), 0, (struct sockaddr*) server, &server_len); 
    if (file.mess_type != FILE_SENDING_SUCCESS){
        printf("file_sending\n");
        exit(1);
    }
    char* buf_file = (char*) calloc(file.mess_size, sizeof(char));    
    mess_t path;
    recvfrom(socket_udp, (char*) &path, sizeof(path), 0, (struct sockaddr*) server, &server_len);
    if (path.mess_type != PATH_SENDING_SUCCESS){
        printf("path_sending\n");
        exit(1);
    }
    char* path_name = (char*) calloc(path.mess_size, sizeof(char));
    
    if (recvfrom(socket_udp, buf_file, file.mess_size, 0, (struct sockaddr*) server, &server_len) != file.mess_size)
        perror("recv message");
    if (recvfrom(socket_udp, path_name, path.mess_size, 0, (struct sockaddr*) server, &server_len) != path.mess_size)
        perror("recv path");
    printf("path_name = %s\n", path_name);
    int file_fd = open(path_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (file_fd < 0) {
        perror("open");
        return -1;
    }
    if (write(file_fd, buf_file, file.mess_size) != file.mess_size)
        perror("write");
    free(path_name);
    free(buf_file);
}

int send_command_udp(struct sockaddr_in* server, int socket_udp) {
    int sz = read(STDIN_FILENO, buf, sizeof(buf));
    if (sz < 0) {
        perror("read");
        return 1;
    }
    //mess_t command = {COMMAND_REQUEST, strlen(buf)};
    //write_mess_udp(socket_udp, command, buf, server);
    sendto(socket_udp, buf, strlen(buf), server, 0, sizeof(*server));
    //mess_t answer;
    socklen_t server_len = sizeof(server);
    //recvfrom(socket_udp, (char*) &answer, sizeof(answer), 0, (struct sockaddr*) server, &server_len);
    /*if (answer.mess_type != COMMAND_SUCCESS) {
        log_error("COMMAND_RECEIVER");
        exit(1);
    }*/
    char* mybuf = (char*) calloc(1000, sizeof(char));
    recvfrom(socket_udp, (char*) mybuf, 1000, 0, (struct sockaddr*) server, &server_len);
    mybuf[strlen(mybuf)] = '\n';
    if (!write(STDOUT_FILENO, mybuf, 1000)) {
        printf("write\n");
        exit(1);
    }
}