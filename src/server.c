#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int init_socket(in_port_t port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in host_addr;
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(port);

    inet_pton(AF_INET, "127.0.0.1", &(host_addr.sin_addr.s_addr));

    bind(sockfd, (struct sockaddr *)&host_addr, sizeof(host_addr));

    listen(sockfd, 10);

    return sockfd;
}

void serve() {
    uint8_t buffer[1024];
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_len = sizeof(struct sockaddr_in);
    int cli_sockfd;
    int sockfd = init_socket(8080);

    cli_sockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_addr_len);

    int len = recv(cli_sockfd, buffer, 1024, 0);

    printf("%s\n", (char *)buffer);
}

int main(int argc, const char *argv[]) {
    serve(atoi(argv[1]));

    return 0;
}