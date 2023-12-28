#include "comm.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BACKLOG 10

int host_get_sock_fd(char *here_port) {
    struct addrinfo *available, *p;
    int sock_fd;
    int yes = 1;

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, here_port, &hints, &available) != 0
        || available == NULL) {
        exit(EXIT_FAILURE);
    }
    for (p = available; p != NULL; p = p->ai_next) {
        if ((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
            == -1) {
            continue;
        }
        setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock_fd);
            continue;
        }
        break;
    }
    freeaddrinfo(available);
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof their_addr;
    listen(sock_fd, BACKLOG);
    int new_fd = accept(sock_fd, (struct sockaddr *)&their_addr, &addr_size);
    close(sock_fd);
    return new_fd;
}

int guest_get_sock_fd(char *there_ip, char *there_port) {
    struct addrinfo *available, *p;
    int sock_fd;

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(there_ip, there_port, &hints, &available) != 0) {
        exit(EXIT_FAILURE);
    }
    for (p = available; p != NULL; p = p->ai_next) {
        if ((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
            == -1) {
            continue;
        }
        while (connect(sock_fd, p->ai_addr, p->ai_addrlen) < 0) {
        }
        break;
    }
    freeaddrinfo(available);
    return sock_fd;
}

int send_data(int fd, void *buff, int size) {
    return send(fd, buff, size, 0);
}

int recv_data(int fd, void *buff, int size) {
    return recv(fd, buff, size, 0);
}
