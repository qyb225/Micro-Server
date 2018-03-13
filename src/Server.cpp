#include "Server.h"
#include <iostream>

Server::Server(in_port_t port) {
    server_sockfd = init_socket(port);
    epollfd = epoll_create(FD_NUM);
}

void Server::run() {
    struct epoll_event events[EPOLL_EVENT_NUM];

    event_register(server_sockfd, EPOLLIN);

    while (1) {
        int events_num = epoll_wait(epollfd, events, EPOLL_EVENT_NUM, -1);
        handle_events(events, events_num);
    }
}

Server::~Server() {
    close(epollfd);
}

void Server::event_register(int fd, int state) {
    struct epoll_event event;
    event.events = state;
    event.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
}

void Server::event_cancel(int fd, int state) {
    struct epoll_event event;
    event.events = state;
    event.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &event);
}

void Server::accept_client() {
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_len = sizeof(struct sockaddr_in);
    int cli_sockfd;

    cli_sockfd = accept(server_sockfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
    if (cli_sockfd != -1) {
        std::cout << "New client accepted: " 
                  << inet_ntoa(cli_addr.sin_addr)
                  << std::endl;
        event_register(cli_sockfd, EPOLLIN);
    }
}

void Server::handle_events(struct epoll_event *events, 
                           int events_num) {
    int fd;
    for (int i = 0; i < events_num; ++i) {
        fd = events[i].data.fd;
        if (fd == server_sockfd && (events[i].events & EPOLLIN)) {
            accept_client();
        } else if (events[i].events & EPOLLIN) {
            get_resquest(fd);
        } else if (events[i].events & EPOLLOUT) {
            continue;
        }
    }
}

void Server::get_resquest(int client_fd) {
    const int HTTP_BUF_SIZE = 1024;
    char http_request_buffer[HTTP_BUF_SIZE];
    int resquest_len = recv(client_fd, http_request_buffer, HTTP_BUF_SIZE, 0);
    if (resquest_len > 0) {
        printf("%s\n", http_request_buffer);
        handle_request(client_fd, http_request_buffer);
    }
}

void Server::handle_request(int client_fd, const char *http_request) {
    char msg[] =  "HTTP/1.0 200 OK <\r\n\r\n<html><h1>Hello, client!</h1></html>\r\n";
    send(client_fd, msg, sizeof(msg), 0);
    close(client_fd);
}

void Server::analysis_request(const char *http_request) {

}

/*Utils*/
int init_socket(in_port_t port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    set_fl(sockfd, O_NONBLOCK);

    struct sockaddr_in host_addr;
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(port);
    inet_pton(AF_INET, IP_ADDR, &(host_addr.sin_addr.s_addr));

    bind(sockfd, (struct sockaddr *)&host_addr, sizeof(host_addr));
    listen(sockfd, LISTEN_Q);

    return sockfd;
}

void set_fl(int fd, int flags) {
    int val = fcntl(fd, F_GETFL, 0);
    val |= flags;
    fcntl(fd, F_SETFL, val);
}