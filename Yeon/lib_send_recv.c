#define _GNU_SOURCE
#include <sys/socket.h>


ssize_t send(int sockfd, const void buf[.len], size_t len, int flags){
    printf("send !\n");
}

