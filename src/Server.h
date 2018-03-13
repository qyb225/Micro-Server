#ifndef SERVER_H
#define SERVER_H
#include <cstdlib>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define IP_ADDR "127.0.0.1"
#define LISTEN_Q 10
#define EPOLL_EVENT_NUM 100
#define FD_NUM 1000
#define BUF_SIZE 4096

class Server {
private:
    int server_sockfd;
    int epollfd;
    char buffer[BUF_SIZE];

    void event_register(int fd, int state);
    void event_cancel(int fd, int state);
    void handle_events(struct epoll_event *events, int events_num);
    void accept_client();
    void get_resquest(int client_fd);

    static void analysis_request(const char *http_request);
    static void handle_request(int client_fd, const char *http_request);
public:
    Server(in_port_t port);
    void run();
    ~Server();
};

int init_socket(in_port_t port);
void set_fl(int fd, int flags);

#endif