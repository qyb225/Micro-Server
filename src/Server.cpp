#include "Server.h"
#include "HttpReq.h"
#include "HttpRes.h"
#include <iostream>
#include <sys/wait.h>

Server::Server(in_port_t port) {
    server_sockfd = init_socket(port);
    epollfd = epoll_create(FD_NUM);
}

void Server::run() {
    struct epoll_event events[EPOLL_EVENT_NUM];

    pid_t master_pid = create_workers(WORKER_NUM);
    event_register(server_sockfd, EPOLLIN);

    if (getpid() == master_pid) {
        /*Master process*/
        while (1) {
            wait(NULL);
            std::cerr << "A worker exit!" << std::endl;
            pid_t pid = fork();
            if (pid == 0) {
                worker_run(events);
            } else if (pid < 0) {
                std::cerr << "Fork error!" << std::endl;
            }
            std::cout << "Worker restart!" << std::endl;
        }
    } else {
        worker_run(events);
    }
}

void Server::worker_run(struct epoll_event *events) {
    while (1) {
        int events_num = epoll_wait(epollfd, events, EPOLL_EVENT_NUM, -1);
        handle_events(events, events_num);
    }
}

pid_t Server::create_workers(int worker_num) {
    pid_t pid = getpid();
    for (int i = 0; i < worker_num; ++i) {
        pid_t child_pid = fork();
        if (child_pid < 0) {
            std::cerr << "Fork error" << std::endl;
        } else if (child_pid == 0) {
            return 0;
        }
    }

    return pid;
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
        std::cout << getpid() << ": New client accepted: " 
                  << inet_ntoa(cli_addr.sin_addr)
                  << std::endl << std::endl;
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
        handle_request(client_fd, http_request_buffer);
    }
}

void Server::handle_request(int client_fd, const char *http_request) {
    HttpReq req(http_request);
    std::string method = req.get_method();
    std::string path = req.get_path();

    HttpRes res;
    std::string res_text = res.exec(path, method);
    send(client_fd, res_text.c_str(), res_text.size(), 0);
    close(client_fd);
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