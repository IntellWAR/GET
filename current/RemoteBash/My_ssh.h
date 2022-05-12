#pragma once
#define _XOPEN_SOURCE 600
#define _GNU_SOURCE
#define __USE_BSD
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#define ERROR_EXIT(ret, str)                                                   \
    do {                                                                       \
        if (ret < 0) {                                                         \
            perror(str);                                                       \
            exit(1);                                                           \
        }                                                                      \
    } while (0)

typedef struct message {
    char mess_type;
    unsigned int mess_size;
} mess_t;

enum TYPES_OF_MESSAGES {
    // AUTH_REQUEST = 1,
    // PASSWORD_REQUEST = 2,
    // PASSWORD_MESSAGE = 3,
    // AUTH_SUCCESS = 4,
    // AUTH_ERROR = 5,
    // AUTH_WRONG_USER = 14,
    // AUTH_WRONG_PASSWORD = 6,
    FILE_PATH = 7,
    FILE_BUF = 8,
    FILE_REQUEST = 9,
    FILE_SENDING_SUCCESS = 10,
    PATH_SENDING_SUCCESS = 11,
    COMMAND_REQUEST = 12,
    COMMAND_SUCCESS = 13
};