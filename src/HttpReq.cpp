#include "HttpReq.h"
#include <sstream>
#include <cstring>


/*目前仅解析首行*/
void HttpReq::handle_request(const char *http_request) {
    std::stringstream stream;
    stream << http_request;

    char line_buffer[256];
    stream.getline(line_buffer, sizeof(line_buffer));

    std::stringstream line;
    line << line_buffer;
    line >> method;
    line >> path;
    line >> version;
}

HttpReq::HttpReq(const char *http_request):
        method("GET"), path("/"), version("HTTP/1.1") {
    handle_request(http_request);
}

std::string HttpReq::get_method() const {
    return method;
}

std::string HttpReq::get_path() const {
    return path;
}

std::string HttpReq::get_version() const {
    return version;
}