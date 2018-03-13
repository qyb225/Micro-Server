#include "Server.h"
#include <cstdlib>

int main(int argc, const char *argv[]) {
    in_port_t port = atoi(argv[1]);
    Server ser(port);
    ser.run();

    return 0;
}